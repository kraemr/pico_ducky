set(PROJECT pico_ducky)

cmake_minimum_required(VERSION 3.12)
set(PICO_BOARD pico2)
set(PICO_PLATFORM rp2350)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(${PROJECT} C CXX)

pico_sdk_init()

add_executable(${PROJECT})

target_sources(${PROJECT} PUBLIC
 ${CMAKE_CURRENT_LIST_DIR}/src/main.c
 ${CMAKE_CURRENT_LIST_DIR}/src/usb_descriptors.c
 ${CMAKE_CURRENT_LIST_DIR}/src/rubberducky.c
 ${CMAKE_CURRENT_LIST_DIR}/src/time_dummy.c
 ${CMAKE_CURRENT_LIST_DIR}/src/spi-fatfs.c
 ${CMAKE_CURRENT_LIST_DIR}/src/spi-fatfs-hwconfig.c
 ${CMAKE_CURRENT_LIST_DIR}/src/msc_disk.c
 ${CMAKE_CURRENT_LIST_DIR}/src/usb_cdc.c
 ${CMAKE_CURRENT_LIST_DIR}/usb_script/constants.c
 ${CMAKE_CURRENT_LIST_DIR}/usb_script/parser.c
)

add_subdirectory(no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI build)

# Make sure TinyUSB can find tusb_config.h
target_include_directories(
        pico_ducky 
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/inc
)

pico_enable_stdio_usb(pico_ducky 0)
pico_enable_stdio_uart(pico_ducky 0)

target_include_directories(pico_ducky PUBLIC ${CMAKE_CURRENT_LIST_DIR})

pico_add_extra_outputs(${PROJECT})

target_link_libraries(
        pico_ducky 
        PUBLIC 
        pico_stdlib
        hardware_pio
        hardware_spi 
        hardware_dma
        FatFs_SPI
        tinyusb_board
        tinyusb_device
)




