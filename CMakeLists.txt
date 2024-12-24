set(PROJECT pico_ducky)

cmake_minimum_required(VERSION 3.12)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(${PROJECT} C CXX)
pico_sdk_init()

add_executable(${PROJECT})
target_sources(${PROJECT} PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/main.c
  ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
  ${CMAKE_CURRENT_LIST_DIR}/rubberducky.c
  ${CMAKE_CURRENT_LIST_DIR}/config.c

)

# Make sure TinyUSB can find tusb_config.h
target_include_directories(pico_ducky PUBLIC
        ${CMAKE_CURRENT_LIST_DIR})


target_include_directories(pico_ducky PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(pico_ducky PUBLIC pico_stdlib pico_unique_id tinyusb_device tinyusb_board)
pico_enable_stdio_usb(pico_ducky 0)

pico_add_extra_outputs(${PROJECT})