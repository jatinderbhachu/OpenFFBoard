#include "CANPort2B.h"
#include "driver/twai.h"
#include "esp_log.h"
#include "stm32_esp32_wrapper.h"


static const char *TAG = "TWAI";

static CAN_TypeDef hcan1_instance;
CAN_HandleTypeDef hcan1 = {
    .Instance = &hcan1_instance,
};

const auto canpresetentries = std::to_array<CANPortHardwareConfig::PresetEntry>(
    {{0x001b0037, 50000, "50k"},
     {0x001b001b, 100000, "100k"},
     {0x001c0014, 125000, "125k"},
     {0x001a000b, 250000, "250k"},
     {0x001a0005, 500000, "500k"},
     {0x001a0002, 1000000, "1000k"}});

CANPortHardwareConfig canpresets =
    CANPortHardwareConfig(true, canpresetentries);
// const OutputPin canSilentPin = OutputPin(*CAN_S_GPIO_Port, CAN_S_Pin);
CANPort_2B canport_base{hcan1, canpresets};
CANPort &canport = static_cast<CANPort &>(canport_base);

HAL_StatusTypeDef HAL_CAN_Start(CAN_HandleTypeDef *hcan) {
  ESP_LOGI(TAG, "%s btr %lu", __FUNCTION__, hcan->Instance->BTR);

  return HAL_OK;
}

HAL_StatusTypeDef HAL_CAN_Stop(CAN_HandleTypeDef *hcan) {
  ESP_LOGI(TAG, "%s", __FUNCTION__);

  return HAL_OK;
}

HAL_StatusTypeDef HAL_CAN_ActivateNotification(CAN_HandleTypeDef *hcan,
                                               uint32_t ActiveITs) {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
  return HAL_OK;
}

HAL_StatusTypeDef HAL_CAN_AddTxMessage(CAN_HandleTypeDef *hcan,
                                       const CAN_TxHeaderTypeDef *pHeader,
                                       const uint8_t aData[],
                                       uint32_t *pTxMailbox) {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
  return HAL_ERROR;
}

HAL_StatusTypeDef HAL_CAN_AbortTxRequest(CAN_HandleTypeDef *hcan,
                                         uint32_t TxMailboxes) {
  ESP_LOGI(TAG, "%s %p %lu", __FUNCTION__, hcan, TxMailboxes);

  return HAL_OK;
}

uint32_t HAL_CAN_GetTxMailboxesFreeLevel(const CAN_HandleTypeDef *hcan) {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
  return 0;
}

uint32_t HAL_CAN_GetTxTimestamp(const CAN_HandleTypeDef *hcan,
                                uint32_t TxMailbox) {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
  return 0;
}

HAL_StatusTypeDef HAL_CAN_GetRxMessage(CAN_HandleTypeDef *hcan, uint32_t RxFifo,
                                       CAN_RxHeaderTypeDef *pHeader,
                                       uint8_t aData[]) {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
  return HAL_ERROR;
}

uint32_t HAL_CAN_GetRxFifoFillLevel(const CAN_HandleTypeDef *hcan,
                                    uint32_t RxFifo) {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
  return 0;
}
HAL_StatusTypeDef HAL_CAN_ConfigFilter(CAN_HandleTypeDef *hcan,
                                       const CAN_FilterTypeDef *sFilterConfig) {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
  return HAL_ERROR;
}

HAL_StatusTypeDef HAL_CAN_ResetError(CAN_HandleTypeDef *hcan) {
  ESP_LOGI(TAG, "%s", __FUNCTION__);
  return HAL_OK;
}