#include "cppmain.h"
#include "ESPNowDriverProtocol.h"
#include "cmsis_os.h"
#include "cpp_target_config.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_now.h"
#include "esp_private/periph_ctrl.h"
#include "esp_private/usb_phy.h"
#include "esp_timer.h"
#include "esp_wifi.h"
#include "flash_helpers.h"
#include "global_callbacks.h"
#include "hal/gpio_types.h"
#include "hal/wdt_hal.h"
#include "nvs.h"
#include "nvs_flash.h"
#include <esp_private/wifi.h>

#include "main.h"
#include "mainclass_chooser.h"
#include "tusb.h"
static const char *TAG = "cppmain";

uint16_t main_id = 0;

FFBoardMain *mainclass __attribute__((section(CCRAM_SEC)));
ClassChooser<FFBoardMain> mainchooser(class_registry);

#define USBD_STACK_SIZE (3 * configMINIMAL_STACK_SIZE / 2)
StackType_t usb_device_stack[USBD_STACK_SIZE];
StaticTask_t usb_device_taskdef;

void cppmain() {
  // Flash init
  if (!Flash_Init()) {
    Error_Handler();
  }

  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES ||
      err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    // NVS partition was truncated and needs to be erased
    // Retry nvs_flash_init
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK(err);

  gpio_install_isr_service(0);
  periph_module_enable(PERIPH_USB_MODULE);
  periph_module_reset(PERIPH_USB_MODULE);
  static usb_phy_handle_t phy_hdl;

  // Configure USB PHY
  usb_phy_config_t phy_conf{
      .controller = USB_PHY_CTRL_OTG,
      .target = USB_PHY_TARGET_INT,
      .otg_mode = USB_OTG_MODE_DEVICE,
      // .otg_speed = USB_PHY_SPEED_UNDEFINED, // autodetect
      .otg_speed = USB_PHY_SPEED_FULL};

  usb_new_phy(&phy_conf, &phy_hdl);

  // Check if flash is initialized
  uint16_t lastFlashVersion = 0;
  if (!Flash_Read(ADR_FLASH_VERSION,
                  &lastFlashVersion)) { // Version never written
    Flash_Write(ADR_FLASH_VERSION, FLASH_VERSION);
  }
  Flash_Read(ADR_FLASH_VERSION, &lastFlashVersion);
  if (lastFlashVersion != FLASH_VERSION) {
    Flash_Format(); // Major version changed or could not write initial value.
                    // force a format
    Flash_Write(ADR_FLASH_VERSION, FLASH_VERSION);
  }

  {
    gpio_config_t io_conf = {0};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask =
        (BIT64(LED_CLIP_Pin) | BIT64(LED_ERR_Pin) | BIT64(LED_SYS_Pin) |
         BIT64(DRV_ENABLE_Pin) | BIT64(DRV_BRAKE_Pin) | BIT64(CAN_S_Pin));
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);
    io_conf.mode = (gpio_mode_t)GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pin_bit_mask = BIT64(BUTTON_A_Pin) | BIT64(DIN2_Pin) |
                           BIT64(DIN1_Pin) | BIT64(DIN0_Pin);
#if DIN3_Pin < SOC_GPIO_PIN_COUNT
    io_conf.pin_bit_mask |= BIT64(DIN3_Pin);
#endif

    gpio_config(&io_conf);

    HAL_GPIO_WritePin(GPIOA, CAN_S_Pin, GPIO_PIN_RESET);
    HAL_CAN_Start(&hcan1);
  }

  {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    cfg.ampdu_tx_enable = 0;
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(ESPNOW_WIFI_MODE));
    ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(
        esp_wifi_set_channel(ESPNOW_CHANNEL, WIFI_SECOND_CHAN_NONE));
    ESP_ERROR_CHECK(esp_wifi_internal_set_fix_rate(ESPNOW_WIFI_IF, true,
                                                   ESPNOW_WIFI_PHY_RATE));
    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_set_pmk((uint8_t *)ESPNOW_PMK));
  }

  // ------------------------

  startADC(); // enable ADC DMA

  // If switch pressed at boot select failsafe implementation
#ifdef BTNFAILSAFE
  if (HAL_GPIO_ReadPin(BUTTON_A_GPIO_Port, BUTTON_A_Pin) == 1) {
    main_id = 0;
  } else
#endif
      if (!Flash_ReadWriteDefault(ADR_CURRENT_CONFIG, &main_id, DEFAULTMAIN)) {
    Error_Handler();
  }

  PersistentStorage::restoreFlashStartupCb(); // Flash is initialized. allow
                                              // restoring now

  mainclass = mainchooser.Create(main_id);
  if (mainclass == nullptr) {          // invalid id
    mainclass = mainchooser.Create(0); // Baseclass
  }

  mainclass->usbInit(); // Let mainclass initialize usb

  while (true) {
    mainclass->update();
    updateLeds();
    // external_spi.process();
    refreshWatchdog();
    taskYIELD(); // Change task if higher priority task wants to run
  }
}

void refreshWatchdog() {
#ifdef HAL_IWDG_MODULE_ENABLED
  HAL_IWDG_Refresh(&hiwdg); // Refresh watchdog
#endif
  vTaskDelay(1);

  wdt_hal_context_t rtc_wdt_ctx = RWDT_HAL_CONTEXT_DEFAULT();
  wdt_hal_write_protect_disable(&rtc_wdt_ctx);
  wdt_hal_feed(&rtc_wdt_ctx);
  wdt_hal_write_protect_enable(&rtc_wdt_ctx);
}

/**
 * TIM_MICROS_HALTICK MUST be reset by the HAL tick OR be the same tick timer to
 * count microseconds since last tick update. By default ST HAL initializes the
 * tick timer with 1MHz and 1kHz overrun interrupts so TIM_MICROS_HALTICK can be
 * defined as that timer. Alternatively an actual freerunning 32b can be defined
 * as TIM_MICROS to use its count directly. Otherwise the cyclecounter is used.
 */
uint32_t micros() { return esp_timer_get_time(); }

/**
 * Helper function for RTOS run time measurements
 * Should return a reasonably accurate and large counter value
 */
unsigned long getRunTimeCounterValue(void) { return micros(); }
