#include "esp_log.h"
#include "nvs_flash.h"
#include "stm32_esp32_wrapper.h"
#include <format>

static const char *TAG = "nvs";
#define NVS_NAMESPACE "storage"

/**
 * @brief  Restore the pages to a known good state in case of page's status
 *   corruption after a power loss.
 * @param  None.
 * @retval - Flash error code: on write Flash error
 *         - FLASH_COMPLETE: on success
 */
uint16_t EE_Init(void) { return HAL_OK; }

/**
 * @brief  Returns the last stored variable data, if found, which correspond to
 *   the passed virtual address
 * @param  VirtAddress: Variable virtual address
 * @param  Data: Global variable contains the read variable value
 * @retval Success or error status:
 *           - 0: if variable was found
 *           - 1: if the variable was not found
 *           - NO_VALID_PAGE: if no valid page was found.
 */
uint16_t EE_ReadVariable(uint16_t VirtAddress, uint16_t *Data) {
  //   ESP_LOGI(TAG, "%s", __FUNCTION__);

  char key[16];
  sprintf(key, "%X", VirtAddress);

  nvs_handle_t handle;
  esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READONLY, &handle);
  err = nvs_get_u16(handle, key, Data);
  nvs_close(handle);

  return err == ESP_OK ? HAL_OK : HAL_ERROR;
}

/**
 * @brief  Writes/upadtes variable data in EEPROM.
 * @param  VirtAddress: Variable virtual address
 * @param  Data: 16 bit data to be written
 * @retval Success or error status:
 *           - FLASH_COMPLETE: on success
 *           - PAGE_FULL: if valid page is full
 *           - NO_VALID_PAGE: if no valid page was found
 *           - Flash error code: on write Flash error
 */
uint16_t EE_WriteVariable(uint16_t VirtAddress, uint16_t Data) {
  //   ESP_LOGI(TAG, "%s", __FUNCTION__);

  char key[16];
  sprintf(key, "%X", VirtAddress);

  nvs_handle_t handle;
  esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &handle);
  err = nvs_set_u16(handle, key, Data);

  nvs_close(handle);

  return err == ESP_OK ? HAL_OK : HAL_ERROR;
}

/**
 * @brief  Erases PAGE and PAGE1 and writes VALID_PAGE header to PAGE
 * @param  None
 * @retval Status of the last operation (Flash write or erase) done during
 *         EEPROM formating
 */
HAL_StatusTypeDef EE_Format(void) {
  //   ESP_LOGI(TAG, "%s", __FUNCTION__);
  ESP_ERROR_CHECK(nvs_flash_erase());
  return HAL_OK;
}
