
file(GLOB FFBOARD_SRC_FILES ../../../FFBoard/Src/*)
file(GLOB FFBOARD_USR_EXT_SRC_FILES ../../../FFBoard/UserExtensions/Src/*)

list(FILTER FFBOARD_SRC_FILES EXCLUDE REGEX ".*cthread\\.cpp$")
list(FILTER FFBOARD_SRC_FILES EXCLUDE REGEX ".*cppmain\\.cpp$")

idf_component_register(
    SRCS 
    "main.c"
    "cppmain.cpp"
    "cthread.cpp"
    "stm32_esp32_wrapper.cpp"
    "eeprom.cpp"
    "can.cpp"
    ${FFBOARD_SRC_FILES}
    ${FFBOARD_USR_EXT_SRC_FILES}
    INCLUDE_DIRS 
    "."
    "../../F407VG/Drivers/CMSIS/Device/ST/STM32F4xx/Include"
    # "../../F407VG/Drivers/CMSIS/Include"
    "./CMSIS_include"
    "../../F407VG/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2"
    "../../F407VG/Drivers/STM32F4xx_HAL_Driver/Inc"
    "../../../FFBoard/Inc"
    "../../../FFBoard/UserExtensions/Inc"

    REQUIRES driver esp_wifi esp_timer nvs_flash
    PRIV_REQUIRES esp_driver_gpio esp_hid usb esp_wifi
)

target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++20 -fexceptions -Wno-ignored-qualifiers -Wno-deprecated-declarations -Wno-missing-field-initializers -Wno-nonnull)

# -Wno-missing-field-initializers -Wno-deprecated-declarations -Wno-unused-variable