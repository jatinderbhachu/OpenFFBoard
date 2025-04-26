#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include <zephyr/kernel.h>

#define STM32F407xx
#include "stm32f4xx_hal.h"

#include "./freertos_wrappers/common.hpp"

#define DIN7_Pin 0
#define DIN7_GPIO_Port 0
#define DIN6_Pin 0
#define DIN6_GPIO_Port 0
#define DIN5_Pin 0
#define DIN5_GPIO_Port 0
#define DIN4_Pin 0
#define DIN4_GPIO_Port 0
#define DIN3_Pin 0
#define DIN3_GPIO_Port 0
#define DIN2_Pin 0
#define DIN2_GPIO_Port 0
#define DIN1_Pin 0
#define DIN1_GPIO_Port 0
#define DIN0_Pin 0
#define DIN0_GPIO_Port 0

#define AIN2_Pin 0
#define AIN2_GPIO_Port 0
#define AIN1_Pin 0
#define AIN1_GPIO_Port 0
#define AIN0_Pin 0
#define AIN0_GPIO_Port 0

#define SPI1_SCK_Pin 0
#define SPI1_MISO_Pin 0
#define SPI1_MOSI_Pin 0
#define SPI1_SS1_Pin 0
#define SPI1_SS1_GPIO_Port 0
#define SPI1_SS2_Pin 0
#define SPI1_SS2_GPIO_Port 0
#define SPI1_SS3_Pin 0
#define SPI1_SS3_GPIO_Port 0

#define BUTTON_A_Pin 0
#define BUTTON_A_GPIO_Port 0

#define DRV_ENABLE_Pin 0
#define DRV_ENABLE_GPIO_Port 0
#define FLAG_Pin 0
#define FLAG_GPIO_Port 0
#define DRV_GP1_Pin 0
#define DRV_GP1_GPIO_Port 0
#define DRV_BRAKE_Pin 0
#define DRV_BRAKE_GPIO_Port 0

#define PWM1_Pin 0
#define PWM1_GPIO_Port 0
#define PWM2_Pin 0
#define PWM2_GPIO_Port 0
#define PWM3_Pin 0
#define PWM3_GPIO_Port 0
#define PWM4_Pin 0
#define PWM4_GPIO_Port 0

#define ENCODER_Z_Pin 0
#define ENCODER_Z_GPIO_Port 0
#define ENCODER_A_Pin 0
#define ENCODER_A_GPIO_Port 0
#define ENCODER_B_Pin 0
#define ENCODER_B_GPIO_Port 0

#define CAN_RX_Pin 0
#define CAN_TX_Pin 0
#define CAN_S_Pin 0

#define LED_SYS_Pin 0
#define LED_SYS_GPIO_Port 0
#define LED_CLIP_Pin 0
#define LED_CLIP_GPIO_Port 0
#define LED_ERR_Pin 0
#define LED_ERR_GPIO_Port 0

size_t xPortGetFreeHeapSize();
size_t xPortGetMinimumEverFreeHeapSize();

void portYIELD_FROM_ISR(BaseType_t type);

void taskYIELD();

void Error_Handler();

uint16_t EE_Init(void);
uint16_t EE_ReadVariable(uint16_t VirtAddress, uint16_t *Data);
uint16_t EE_WriteVariable(uint16_t VirtAddress, uint16_t Data);
HAL_StatusTypeDef EE_Format();

HAL_StatusTypeDef HAL_FLASH_Unlock();
HAL_StatusTypeDef HAL_FLASH_Lock();

// GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_NUM);
// void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_NUM,
//     GPIO_PinState PinState);
// void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_NUM);
// HAL_StatusTypeDef HAL_GPIO_LockPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_NUM);
// void HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_NUM);

// void HAL_IncTick(void);
// void HAL_Delay(uint32_t Delay);
// uint32_t HAL_GetTick(void);
// uint32_t HAL_GetTickPrio(void);
// HAL_StatusTypeDef HAL_SetTickFreq(HAL_TickFreqTypeDef Freq);
// HAL_TickFreqTypeDef HAL_GetTickFreq(void);
// void HAL_SuspendTick(void);
// void HAL_ResumeTick(void);
// uint32_t HAL_GetHalVersion(void);
// uint32_t HAL_GetREVID(void);
// uint32_t HAL_GetDEVID(void);
// void HAL_DBGMCU_EnableDBGSleepMode(void);
// void HAL_DBGMCU_DisableDBGSleepMode(void);
// void HAL_DBGMCU_EnableDBGStopMode(void);
// void HAL_DBGMCU_DisableDBGStopMode(void);
// void HAL_DBGMCU_EnableDBGStandbyMode(void);
// void HAL_DBGMCU_DisableDBGStandbyMode(void);
// void HAL_EnableCompensationCell(void);
// void HAL_DisableCompensationCell(void);
// uint32_t HAL_GetUIDw0(void);
// uint32_t HAL_GetUIDw1(void);
// uint32_t HAL_GetUIDw2(void);

uint32_t HAL_RCC_GetHCLKFreq(void);

// // HAL_StatusTypeDef HAL_IWDG_Refresh(IWDG_HandleTypeDef *hiwdg);
// HAL_StatusTypeDef HAL_ADC_Start_DMA(ADC_HandleTypeDef *hadc, uint32_t *pData,
//                                     uint32_t Length);
// HAL_StatusTypeDef HAL_ADC_Stop_DMA(ADC_HandleTypeDef *hadc);

void NVIC_SystemReset();
void RebootDFU();

// HAL_StatusTypeDef HAL_CAN_Start(CAN_HandleTypeDef *hcan);
// HAL_StatusTypeDef HAL_CAN_Stop(CAN_HandleTypeDef *hcan);
// HAL_StatusTypeDef HAL_CAN_ActivateNotification(CAN_HandleTypeDef *hcan,
//                                                uint32_t ActiveITs);
// HAL_StatusTypeDef HAL_CAN_AddTxMessage(CAN_HandleTypeDef *hcan,
//                                        const CAN_TxHeaderTypeDef *pHeader,
//                                        const uint8_t aData[],
//                                        uint32_t *pTxMailbox);
// HAL_StatusTypeDef HAL_CAN_AbortTxRequest(CAN_HandleTypeDef *hcan,
//                                          uint32_t TxMailboxes);
// uint32_t HAL_CAN_GetTxMailboxesFreeLevel(const CAN_HandleTypeDef *hcan);
// uint32_t HAL_CAN_GetTxTimestamp(const CAN_HandleTypeDef *hcan,
//                                 uint32_t TxMailbox);
// HAL_StatusTypeDef HAL_CAN_GetRxMessage(CAN_HandleTypeDef *hcan, uint32_t
// RxFifo,
//                                        CAN_RxHeaderTypeDef *pHeader,
//                                        uint8_t aData[]);
// uint32_t HAL_CAN_GetRxFifoFillLevel(const CAN_HandleTypeDef *hcan,
//                                     uint32_t RxFifo);
// HAL_StatusTypeDef HAL_CAN_ConfigFilter(CAN_HandleTypeDef *hcan,
//                                        const CAN_FilterTypeDef
//                                        *sFilterConfig);
// HAL_StatusTypeDef HAL_CAN_ResetError(CAN_HandleTypeDef *hcan);

#ifdef __cplusplus
}
#endif