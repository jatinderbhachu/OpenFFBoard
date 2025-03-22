#include <stdlib.h>
#include "esp_log.h"
#include "esp_check.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"

#include "esp_private/periph_ctrl.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_mac.h"
#include "esp_now.h"
#include "nvs_flash.h"
#include "nvs.h"

#include "cppmain.h"

static const char *TAG = "app_main";

extern void cppmain();

void app_main(void)
{
    cppmain();
}