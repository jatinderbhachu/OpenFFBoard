#include <stdlib.h>
#include <zephyr/drivers/usb/udc.h>

#include <zephyr/dt-bindings/regulator/nrf5x.h>

#include <hal/nrf_usbd.h>
#include <nrfx_clock.h>
#include <nrfx_power.h>
#include <tusb.h>

extern void cppmain();

extern void tusb_hal_nrf_power_event(uint32_t event);

static void power_event_handler(nrfx_power_usb_evt_t event) {
  tusb_hal_nrf_power_event((uint32_t)event);
}

void USBD_IRQHandler(void) { tud_int_handler(0); }
// void USBD_IRQHandler(void) { tusb_int_handler(0, true); }

enum {
  USB_EVT_DETECTED = 0,
  USB_EVT_REMOVED = 1,
  USB_EVT_READY = 2,
};

void init_usb() {
#define DT_DRV_COMPAT nordic_nrf_usbd
  IRQ_CONNECT(DT_INST_IRQN(0), DT_INST_IRQ(0, priority), nrfx_isr,
              USBD_IRQHandler, 0);
  irq_enable(DT_INST_IRQN(0));

  // NVIC_SetPriority(USBD_IRQn, 2);

  uint32_t usb_reg;
  usb_reg = NRF_POWER->USBREGSTATUS;

  {
    // Power module init
    const nrfx_power_config_t pwr_cfg = {0};
    nrfx_power_init(&pwr_cfg);

    // Register tusb function as USB power handler
    // cause cast-function-type warning
    const nrfx_power_usbevt_config_t config = {.handler = power_event_handler};
    nrfx_power_usbevt_init(&config);
    nrfx_power_usbevt_enable();
    usb_reg = NRF_POWER->USBREGSTATUS;
  }

  if (usb_reg & POWER_USBREGSTATUS_VBUSDETECT_Msk) {
    tusb_hal_nrf_power_event(USB_EVT_DETECTED);
  }
  if (usb_reg & POWER_USBREGSTATUS_OUTPUTRDY_Msk) {
    tusb_hal_nrf_power_event(USB_EVT_READY);
  }
}

void main(void) {
  init_usb();
  cppmain();
}