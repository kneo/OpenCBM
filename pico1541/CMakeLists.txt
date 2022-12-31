cmake_minimum_required(VERSION 3.25)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(pico1541 C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(pico1541 pico1541.c board-pico-official.c pico-s1.c pico-iec.c pico-commands.c)

#pico_generate_pio_header(pio_test ${CMAKE_CURRENT_LIST_DIR}/pio_test.pio)

# pull in common dependencies
target_link_libraries(pico1541 pico_stdlib hardware_gpio hardware_pio)

pico_enable_stdio_usb(pico1541 0)
pico_enable_stdio_uart(pico1541 1)

# create map/bin/hex file etc.
pico_add_extra_outputs(pico1541)