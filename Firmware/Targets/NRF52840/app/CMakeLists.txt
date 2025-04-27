cmake_minimum_required(VERSION 3.20.0)

set(BOARD promicro_nrf52840/nrf52840/uf2)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(openffboard)


file(GLOB FFBOARD_SRC_FILES ../../../FFBoard/Src/*)
file(GLOB FFBOARD_USR_EXT_SRC_FILES ../../../FFBoard/UserExtensions/Src/*)


file(GLOB FFBOARD_TINYUSB_SRC_FILES 
    ../../../FFBoard/USB/*.c
    ../../../FFBoard/USB/class/audio/*.c
    ../../../FFBoard/USB/class/bth/*.c
    ../../../FFBoard/USB/class/cdc/*.c
    ../../../FFBoard/USB/class/dfu/*.c
    ../../../FFBoard/USB/class/hid/*.c
    ../../../FFBoard/USB/class/midi/*.c
    ../../../FFBoard/USB/class/msc/*.c
    ../../../FFBoard/USB/class/net/*.c
    ../../../FFBoard/USB/class/usbtmc/*.c
    ../../../FFBoard/USB/class/vendor/*.c
    ../../../FFBoard/USB/common/*.c
    ../../../FFBoard/USB/device/*.c
    ../../../FFBoard/USB/host/*.c
    ../../../FFBoard/USB/portable/nordic/nrf5x/*.c
)

list(FILTER FFBOARD_SRC_FILES EXCLUDE REGEX ".*cthread\\.cpp$")
list(FILTER FFBOARD_SRC_FILES EXCLUDE REGEX ".*cppmain\\.cpp$")
list(FILTER FFBOARD_SRC_FILES EXCLUDE REGEX ".*eeprom\\.c$")

target_include_directories(
    app
    PRIVATE
    src/
    src/stm32_hal/
    ../../../FFBoard/Inc
    ../../../FFBoard/UserExtensions/Inc
    ../../../FFBoard/USB
    ../../../FFBoard/USB/device
    ../../../FFBoard/USB/portable/synopsys/dwc2/
    ../../../FFBoard/USB/class/cdc
    ../../../FFBoard/USB/class/midi
    ../../../FFBoard/USB/class/hid

    src/dummy_includes
    src/freertos_wrappers
)

target_sources(
    app 
    PRIVATE

    src/main.c
    src/cppmain.cpp
    src/wrapper.cpp
    src/eeprom.cpp
    src/freertos_wrappers/thread.cpp
    src/freertos_wrappers/mutex.cpp
    src/freertos_wrappers/semaphore.cpp
    ${FFBOARD_SRC_FILES}
    ${FFBOARD_USR_EXT_SRC_FILES}
    ${FFBOARD_TINYUSB_SRC_FILES}
)

# target_compile_options(${COMPONENT_LIB} PRIVATE -std=gnu++20 -fexceptions -Wno-ignored-qualifiers -Wno-deprecated-declarations -Wno-missing-field-initializers -Wno-nonnull)