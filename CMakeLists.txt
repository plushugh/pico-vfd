cmake_minimum_required(VERSION 3.13...3.27)

# initialize the SDK based on PICO_SDK_PATH
include(pico_sdk_import.cmake)


set(CMAKE_EXPORT_COMPILE_COMMANDS 1)


project(vfdtest)

pico_sdk_init()

add_subdirectory(lib/vfd)

add_executable(vfdtest
    src/main.c
)

target_link_libraries(vfdtest
    pico_stdlib
    hardware_spi
    vfd
)

pico_enable_stdio_usb(vfdtest 1)
pico_enable_stdio_uart(vfdtest 0)

pico_add_extra_outputs(vfdtest)
