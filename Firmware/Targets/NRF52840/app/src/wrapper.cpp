#include "wrapper.h"

#include <cstdio>

void Error_Handler()
{
}

size_t xPortGetFreeHeapSize()
{
    return 1024 * 1024;
}

size_t xPortGetMinimumEverFreeHeapSize()
{
    return 1024;
}

uint32_t HAL_RCC_GetHCLKFreq(void)
{
    return 16000000;
}

void RebootDFU()
{
    printf("%s\n", __FUNCTION__);
}

HAL_StatusTypeDef HAL_FLASH_Unlock()
{
    // printf("%s\n", __FUNCTION__);
    return HAL_OK;
}
HAL_StatusTypeDef HAL_FLASH_Lock()
{
    // printf("%s\n", __FUNCTION__);
    return HAL_OK;
}

HAL_StatusTypeDef HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *SectorError)
{
    // printf("%s\n", __FUNCTION__);
    return HAL_OK;
}

HAL_StatusTypeDef HAL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint64_t Data)
{
    // printf("%s\n", __FUNCTION__);
    return HAL_OK;
}

GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    // printf("%s\n", __FUNCTION__);
    return GPIO_PIN_RESET;
}

void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin,
                       GPIO_PinState PinState)
{
    // printf("%s\n", __FUNCTION__);
}

void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    // printf("%s\n", __FUNCTION__);
}

HAL_StatusTypeDef HAL_GPIO_LockPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    // printf("%s\n", __FUNCTION__);
    return HAL_OK;
}

void HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin)
{
    // printf("%s\n", __FUNCTION__);
}

uint32_t HAL_GetUIDw0(void)
{
    // printf("%s\n", __FUNCTION__);
    return 123;
}
uint32_t HAL_GetUIDw1(void)
{
    // printf("%s\n", __FUNCTION__);
    return 456;
}
uint32_t HAL_GetUIDw2(void)
{
    // printf("%s\n", __FUNCTION__);
    return 789;
}

void HAL_IncTick(void)
{
    // printf("%s\n", __FUNCTION__);
}

void HAL_Delay(uint32_t Delay)
{
    // printf("%s\n", __FUNCTION__);
}

uint32_t HAL_GetTick(void)
{
    return k_cyc_to_ms_floor32(k_cycle_get_32());
}

uint32_t HAL_GetTickPrio(void)
{
    // printf("%s\n", __FUNCTION__);
    return 0;
}

HAL_StatusTypeDef HAL_SetTickFreq(HAL_TickFreqTypeDef Freq)
{
    // printf("%s\n", __FUNCTION__);
    return HAL_ERROR;
}

HAL_TickFreqTypeDef HAL_GetTickFreq(void)
{
    // printf("%s\n", __FUNCTION__);
    return HAL_TICK_FREQ_1KHZ;
}

void HAL_SuspendTick(void)
{
    // printf("%s\n", __FUNCTION__);
}

void HAL_ResumeTick(void)
{
    // printf("%s\n", __FUNCTION__);
}

uint32_t HAL_GetHalVersion(void)
{
    // printf("%s\n", __FUNCTION__);
    return 0;
}

uint32_t HAL_GetREVID(void)
{
    // printf("%s\n", __FUNCTION__);
    return 0;
}

uint32_t HAL_GetDEVID(void)
{
    // printf("%s\n", __FUNCTION__);
    return 0x413;
}