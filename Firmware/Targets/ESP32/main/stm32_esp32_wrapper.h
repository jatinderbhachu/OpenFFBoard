#pragma once

#define STM32F407xx

#include "driver/gpio.h"

#include "stm32f4xx_hal.h"

// Pin assignments originally from https://github.com/TDA-2030/OpenFFBoard/blob/4b539209f10d7ea58eaa769e839fda7e71bccd3c/Firmware/Targets/ESP32SX/main/openffboard_esp32s3_v1.1_pins.h
#define DIN7_Pin 255 // Define a invalid value
#define DIN7_GPIO_Port GPIOE
#define DIN6_Pin 255 // Define a invalid value
#define DIN6_GPIO_Port GPIOE
#define DIN5_Pin 255 // Define a invalid value
#define DIN5_GPIO_Port GPIOE
#define DIN4_Pin 255 // Define a invalid value
#define DIN4_GPIO_Port GPIOE
#define DIN3_Pin GPIO_NUM_8
#define DIN3_GPIO_Port GPIOE
#define DIN2_Pin GPIO_NUM_9
#define DIN2_GPIO_Port GPIOC
#define DIN1_Pin GPIO_NUM_10
#define DIN1_GPIO_Port GPIOC
#define DIN0_Pin GPIO_NUM_11
#define DIN0_GPIO_Port GPIOC

#define AIN2_Pin GPIO_NUM_3
#define AIN2_GPIO_Port GPIOC
#define AIN1_Pin GPIO_NUM_2
#define AIN1_GPIO_Port GPIOA
#define AIN0_Pin GPIO_NUM_1
#define AIN0_GPIO_Port GPIOA

#define SPI1_SCK_Pin GPIO_NUM_46
#define SPI1_MISO_Pin GPIO_NUM_38
#define SPI1_MOSI_Pin GPIO_NUM_39
#define SPI1_SS1_Pin GPIO_NUM_45
#define SPI1_SS1_GPIO_Port GPIOA
#define SPI1_SS2_Pin GPIO_NUM_47
#define SPI1_SS2_GPIO_Port GPIOB
#define SPI1_SS3_Pin GPIO_NUM_48
#define SPI1_SS3_GPIO_Port GPIOB

#define BUTTON_A_Pin GPIO_NUM_0
#define BUTTON_A_GPIO_Port GPIOB

#define DRV_ENABLE_Pin GPIO_NUM_13
#define DRV_ENABLE_GPIO_Port GPIOE
#define FLAG_Pin GPIO_NUM_14
#define FLAG_GPIO_Port GPIOE
#define DRV_GP1_Pin GPIO_NUM_12
#define DRV_GP1_GPIO_Port GPIOE
#define DRV_BRAKE_Pin GPIO_NUM_21
#define DRV_BRAKE_GPIO_Port GPIOE

#define PWM1_Pin GPIO_NUM_5
#define PWM1_GPIO_Port GPIOE
#define PWM2_Pin GPIO_NUM_7
#define PWM2_GPIO_Port GPIOE
#define PWM3_Pin GPIO_NUM_16
#define PWM3_GPIO_Port GPIOE
#define PWM4_Pin GPIO_NUM_17
#define PWM4_GPIO_Port GPIOE

#define ENCODER_Z_Pin GPIO_NUM_15
#define ENCODER_Z_GPIO_Port GPIOD
#define ENCODER_A_Pin GPIO_NUM_4
#define ENCODER_A_GPIO_Port GPIOC
#define ENCODER_B_Pin GPIO_NUM_6
#define ENCODER_B_GPIO_Port GPIOC

#define CAN_RX_Pin GPIO_NUM_35
#define CAN_TX_Pin GPIO_NUM_36
#define CAN_S_Pin GPIO_NUM_37


#define LED_SYS_Pin GPIO_NUM_42
#define LED_SYS_GPIO_Port GPIOD
#define LED_CLIP_Pin GPIO_NUM_41
#define LED_CLIP_GPIO_Port GPIOE
#define LED_ERR_Pin GPIO_NUM_40
#define LED_ERR_GPIO_Port GPIOE

#ifdef __cplusplus
extern "C" {
#endif

// HACK: In esp-idf these functions take a spinlock as a paramter so we just override it to our own wrapper implementation
// The compiler doesnt like it though
// https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/freertos_idf.html#api-changes 
void wrapper_taskENTER_CRITICAL();
void wrapper_taskEXIT_CRITICAL();

#define taskENTER_CRITICAL() wrapper_taskENTER_CRITICAL()
#define taskEXIT_CRITICAL() wrapper_taskEXIT_CRITICAL()

void Error_Handler();

uint16_t EE_Init(void);
uint16_t EE_ReadVariable(uint16_t VirtAddress, uint16_t *Data);
uint16_t EE_WriteVariable(uint16_t VirtAddress, uint16_t Data);
HAL_StatusTypeDef EE_Format();

HAL_StatusTypeDef HAL_FLASH_Unlock();
HAL_StatusTypeDef HAL_FLASH_Lock();

GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_NUM);
void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_NUM,
    GPIO_PinState PinState);
void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_NUM);
HAL_StatusTypeDef HAL_GPIO_LockPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_NUM);
void HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_NUM);

void HAL_IncTick(void);
void HAL_Delay(uint32_t Delay);
uint32_t HAL_GetTick(void);
uint32_t HAL_GetTickPrio(void);
HAL_StatusTypeDef HAL_SetTickFreq(HAL_TickFreqTypeDef Freq);
HAL_TickFreqTypeDef HAL_GetTickFreq(void);
void HAL_SuspendTick(void);
void HAL_ResumeTick(void);
uint32_t HAL_GetHalVersion(void);
uint32_t HAL_GetREVID(void);
uint32_t HAL_GetDEVID(void);
void HAL_DBGMCU_EnableDBGSleepMode(void);
void HAL_DBGMCU_DisableDBGSleepMode(void);
void HAL_DBGMCU_EnableDBGStopMode(void);
void HAL_DBGMCU_DisableDBGStopMode(void);
void HAL_DBGMCU_EnableDBGStandbyMode(void);
void HAL_DBGMCU_DisableDBGStandbyMode(void);
void HAL_EnableCompensationCell(void);
void HAL_DisableCompensationCell(void);
uint32_t HAL_GetUIDw0(void);
uint32_t HAL_GetUIDw1(void);
uint32_t HAL_GetUIDw2(void);

uint32_t HAL_RCC_GetHCLKFreq(void);
// HAL_StatusTypeDef HAL_IWDG_Refresh(IWDG_HandleTypeDef *hiwdg);
HAL_StatusTypeDef HAL_ADC_Start_DMA(ADC_HandleTypeDef *hadc, uint32_t *pData,
                                    uint32_t Length);
HAL_StatusTypeDef HAL_ADC_Stop_DMA(ADC_HandleTypeDef *hadc);

void NVIC_SystemReset();
void RebootDFU();

HAL_StatusTypeDef HAL_CAN_Start(CAN_HandleTypeDef *hcan);
HAL_StatusTypeDef HAL_CAN_Stop(CAN_HandleTypeDef *hcan);
HAL_StatusTypeDef HAL_CAN_ActivateNotification(CAN_HandleTypeDef *hcan,
                                               uint32_t ActiveITs);
HAL_StatusTypeDef HAL_CAN_AddTxMessage(CAN_HandleTypeDef *hcan,
                                       const CAN_TxHeaderTypeDef *pHeader,
                                       const uint8_t aData[],
                                       uint32_t *pTxMailbox);
HAL_StatusTypeDef HAL_CAN_AbortTxRequest(CAN_HandleTypeDef *hcan,
                                         uint32_t TxMailboxes);
uint32_t HAL_CAN_GetTxMailboxesFreeLevel(const CAN_HandleTypeDef *hcan);
uint32_t HAL_CAN_GetTxTimestamp(const CAN_HandleTypeDef *hcan,
                                uint32_t TxMailbox);
HAL_StatusTypeDef HAL_CAN_GetRxMessage(CAN_HandleTypeDef *hcan, uint32_t RxFifo,
                                       CAN_RxHeaderTypeDef *pHeader,
                                       uint8_t aData[]);
uint32_t HAL_CAN_GetRxFifoFillLevel(const CAN_HandleTypeDef *hcan,
                                    uint32_t RxFifo);
HAL_StatusTypeDef HAL_CAN_ConfigFilter(CAN_HandleTypeDef *hcan,
                                       const CAN_FilterTypeDef *sFilterConfig);
HAL_StatusTypeDef HAL_CAN_ResetError(CAN_HandleTypeDef *hcan);

#ifdef __cplusplus
}
#endif