set(PROJECT pico_ducky)

cmake_minimum_required(VERSION 3.12)
set(PICO_BOARD pico2)
set(PICO_PLATFORM rp2350)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

#set(TINYUSB_DEVICE_STACK 1)
#set(TINYUSB_HOST_STACK 0)

project(${PROJECT} C CXX)

pico_sdk_init()
add_executable(${PROJECT})
target_sources(${PROJECT} PUBLIC
 #${CMAKE_CURRENT_LIST_DIR}/main.c
 #${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
 #${CMAKE_CURRENT_LIST_DIR}/rubberducky.c
 ${CMAKE_CURRENT_LIST_DIR}/parser.c
 #${CMAKE_CURRENT_LIST_DIR}/config.c
 ${CMAKE_CURRENT_LIST_DIR}/time_dummy.c
 ${CMAKE_CURRENT_LIST_DIR}/spi-fatfs.c
 ${CMAKE_CURRENT_LIST_DIR}/spi-fatfs-hwconfig.c
)

add_subdirectory(no-OS-FatFS-SD-SPI-RPi-Pico/FatFs_SPI build)

# Make sure TinyUSB can find tusb_config.h
target_include_directories(
        pico_ducky 
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
)

pico_enable_stdio_usb(pico_ducky 1)
pico_enable_stdio_uart(pico_ducky 0)

target_include_directories(pico_ducky PUBLIC ${CMAKE_CURRENT_LIST_DIR})
pico_add_extra_outputs(${PROJECT})

target_link_libraries(
        pico_ducky 
        PUBLIC 
        pico_stdlib 
        hardware_spi 
        hardware_dma
        FatFs_SPI
        #tinyusb_board
        #tinyusb_device
)




