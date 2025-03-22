#include "stm32_esp32_wrapper.h"
#include "CANPort2B.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_timer.h"
#include <string.h>

static const char *TAG = "WRAPPER";

static portMUX_TYPE my_spinlock = portMUX_INITIALIZER_UNLOCKED;

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;
SPI_HandleTypeDef hspi3;

void wrapper_taskENTER_CRITICAL() {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
  taskENTER_CRITICAL(&my_spinlock);
}
void wrapper_taskEXIT_CRITICAL() {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
  taskEXIT_CRITICAL(&my_spinlock);
}

void Error_Handler() { ESP_LOGI(TAG, "%s", __FUNCTION__); }

HAL_StatusTypeDef HAL_FLASH_Unlock() {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
  return HAL_OK;
}
HAL_StatusTypeDef HAL_FLASH_Lock() {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
  return HAL_OK;
}

GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
  return GPIO_PIN_RESET;
}

void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,
                       GPIO_PinState PinState) {
  // ESP_LOGI(TAG, "%s", __FUNCTION__);
}

void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
  // ESP_LOGI(TAG, "%s", __FUNCTION__);
}

HAL_StatusTypeDef HAL_GPIO_LockPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin) {
  // ESP_LOGI(TAG, "%s", __FUNCTION__);
  return HAL_ERROR;
}

void HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin) {
  // ESP_LOGI(TAG, "%s", __FUNCTION__);
}

void HAL_IncTick(void) { ESP_LOGI(TAG, "%s", __FUNCTION__); }

void HAL_Delay(uint32_t Delay) { ESP_LOGI(TAG, "%s", __FUNCTION__); }

uint32_t HAL_GetTick(void) {
  //   ESP_LOGI(TAG, "%s", __FUNCTION__);
  return esp_timer_get_time() / 1000;
}

uint32_t HAL_GetTickPrio(void) {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
  return 0;
}

HAL_StatusTypeDef HAL_SetTickFreq(HAL_TickFreqTypeDef Freq) {
  ESP_LOGI(TAG, "%s", __FUNCTION__);

  return HAL_ERROR;
}

HAL_TickFreqTypeDef HAL_GetTickFreq(void) {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
  return HAL_TICK_FREQ_1KHZ;
}

void HAL_SuspendTick(void) { ESP_LOGI(TAG, "%s", __FUNCTION__); }

void HAL_ResumeTick(void) { ESP_LOGI(TAG, "%s", __FUNCTION__); }

uint32_t HAL_GetHalVersion(void) {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
  return 0;
}

uint32_t HAL_GetREVID(void) {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
  return 0;
}

uint32_t HAL_GetDEVID(void) {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
  return FW_DEVID;
  return 0;
}

void HAL_DBGMCU_EnableDBGSleepMode(void) { ESP_LOGI(TAG, "%s", __FUNCTION__); }

void HAL_DBGMCU_DisableDBGSleepMode(void) { ESP_LOGI(TAG, "%s", __FUNCTION__); }

void HAL_DBGMCU_EnableDBGStopMode(void) { ESP_LOGI(TAG, "%s", __FUNCTION__); }

void HAL_DBGMCU_DisableDBGStopMode(void) { ESP_LOGI(TAG, "%s", __FUNCTION__); }

void HAL_DBGMCU_EnableDBGStandbyMode(void) {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
}

void HAL_DBGMCU_DisableDBGStandbyMode(void) {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
}

void HAL_EnableCompensationCell(void) { ESP_LOGI(TAG, "%s", __FUNCTION__); }

void HAL_DisableCompensationCell(void) { ESP_LOGI(TAG, "%s", __FUNCTION__); }

uint32_t HAL_GetUIDw0(void) {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
  return 0;
}
uint32_t HAL_GetUIDw1(void) {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
  return 0;
}
uint32_t HAL_GetUIDw2(void) {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
  return 0;
}
uint32_t HAL_RCC_GetHCLKFreq(void) {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
  return 0;
}

// HAL_StatusTypeDef HAL_IWDG_Refresh(IWDG_HandleTypeDef *hiwdg) {
//   return HAL_ERROR;
// }

HAL_StatusTypeDef HAL_ADC_Start_DMA(ADC_HandleTypeDef *hadc, uint32_t *pData,
                                    uint32_t Length) {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
  return HAL_ERROR;
}

HAL_StatusTypeDef HAL_ADC_Stop_DMA(ADC_HandleTypeDef *hadc) {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
  return HAL_ERROR;
}

void NVIC_SystemReset() {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
  esp_restart();
}

void RebootDFU() { ESP_LOGI(TAG, "%s", __FUNCTION__); }