# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

cmake_minimum_required(VERSION 3.13)

# Requires PICO_SDK_PATH to be set, for example:
# export PICO_SDK_PATH=~/pico/pico-sdk
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(pico_rabbit C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(pico_rabbit
    src/main.c
    src/rabbit_hw.c
    src/morse.c
)

target_include_directories(pico_rabbit PRIVATE
    include
)

target_link_libraries(pico_rabbit
    pico_stdlib
    hardware_pwm
)

pico_enable_stdio_usb(pico_rabbit 1)
pico_enable_stdio_uart(pico_rabbit 0)

pico_add_extra_outputs(pico_rabbit)
