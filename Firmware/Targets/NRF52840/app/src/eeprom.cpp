#include "wrapper.h"

#include <cstdio>
#include <zephyr/kernel.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/fs/nvs.h>

#define FFBOARD_PARTITION storage_partition

#define FFBOARD_PARTITION_OFFSET FIXED_PARTITION_OFFSET(FFBOARD_PARTITION)
#define FFBOARD_PARTITION_DEVICE FIXED_PARTITION_DEVICE(FFBOARD_PARTITION)

static struct nvs_fs fs;

uint16_t EE_Init(void)
{
    int rc = 0;
    struct flash_pages_info info;

    fs.flash_device = FFBOARD_PARTITION_DEVICE;
    if (!device_is_ready(fs.flash_device))
    {
        return HAL_ERROR;
    }
    fs.offset = FFBOARD_PARTITION_OFFSET;
    rc = flash_get_page_info_by_offs(fs.flash_device, fs.offset, &info);
    if (rc)
    {
        return HAL_ERROR;
    }
    fs.sector_size = info.size;
    fs.sector_count = 3U;

    rc = nvs_mount(&fs);
    if (rc)
    {
        return HAL_ERROR;
    }

    return HAL_OK;
}

uint16_t EE_ReadVariable(uint16_t VirtAddress, uint16_t *Data)
{
    int rc = nvs_read(&fs, VirtAddress, Data, sizeof(uint16_t));
    return rc > 0 ? HAL_OK : HAL_ERROR;
}

uint16_t EE_WriteVariable(uint16_t VirtAddress, uint16_t Data)
{
    int rc = nvs_write(&fs, VirtAddress, &Data, sizeof(uint16_t));

    return rc > 0 ? HAL_OK : HAL_ERROR;
}

HAL_StatusTypeDef EE_Format(void)
{
    nvs_clear(&fs);
    return HAL_OK;
}