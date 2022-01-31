cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(ac_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

include_directories(include pico-joybus-comms/include)

add_executable(ac
   src/main.cpp
   src/nunchuk.cpp
   pico-joybus-comms/src/joybusComms.cpp
)

pico_enable_stdio_usb(ac 1)
pico_enable_stdio_uart(ac 1)

pico_add_extra_outputs(ac)

target_link_libraries(ac pico_stdlib hardware_i2c hardware_gpio pico_multicore)
