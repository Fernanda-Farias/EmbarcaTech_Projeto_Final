# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 1.5.1)
set(toolchainVersion 13_2_Rel1)
set(picotoolVersion 2.0.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(pico_sdk_import.cmake)

project(EMBARCATECH_PROJETO_FINAL C CXX ASM)

pico_sdk_init()

add_executable(EMBARCATECH_PROJETO_FINAL 
    main.c
    ssd1306/ssd1306_i2c.c
)

pico_set_program_name(EMBARCATECH_PROJETO_FINAL "EMBARCATECH_PROJETO_FINAL")
pico_set_program_version(EMBARCATECH_PROJETO_FINAL "0.1")

pico_generate_pio_header(EMBARCATECH_PROJETO_FINAL ${CMAKE_CURRENT_LIST_DIR}/pio/ws2818b.pio)

pico_enable_stdio_uart(EMBARCATECH_PROJETO_FINAL 0)
pico_enable_stdio_usb(EMBARCATECH_PROJETO_FINAL 1)

target_link_libraries(EMBARCATECH_PROJETO_FINAL
    hardware_i2c
    pico_stdlib
    hardware_pio
    hardware_pwm
    hardware_clocks
    hardware_adc
)

target_include_directories(EMBARCATECH_PROJETO_FINAL PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/ssd1306
)

pico_add_extra_outputs(EMBARCATECH_PROJETO_FINAL)