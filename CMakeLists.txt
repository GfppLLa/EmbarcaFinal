# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(EmbarcaFinal C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(EmbarcaFinal 
                EmbarcaFinal.c 
                include/modules/escalaAlerta.c
                include/modules/font.c
                include/modules/operacaoPio.c
                include/modules/referencia.c
                include/modules/ssd1306.c
                )

pico_set_program_name(EmbarcaFinal "EmbarcaFinal")
pico_set_program_version(EmbarcaFinal "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_generate_pio_header(EmbarcaFinal ${CMAKE_CURRENT_LIST_DIR}/pio_matrix.pio)

pico_enable_stdio_uart(EmbarcaFinal 0)
pico_enable_stdio_usb(EmbarcaFinal 1)

# Add the standard library to the build
target_link_libraries(EmbarcaFinal
        pico_stdlib
        pico_stdlib          # funções padrão do pico
        hardware_adc         # biblioteca para o uso do adc
        hardware_pwm         # controle do pwm
        hardware_flash       # manipulação da memória flash
        hardware_clocks      # controle dos clocks do pico
        hardware_pio         # controle das máquinas de estado pio
        hardware_structs     # acesso às structs de hardware
        pico_multicore       # suporte para uso dos dois núcleos
        pico_time            # manipulação de tempo e delays
        pico_rand            # geração de números aleatórios
        hardware_i2c
        )
target_sources(EmbarcaFinal PRIVATE
                include/modules/operacaoPio.c
                include/modules/font.c
                include/modules/referencia.c
                include/modules/ssd1306.c
)   
# Add the standard include files to the build
target_include_directories(EmbarcaFinal PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/include/libraries
        ${CMAKE_CURRENT_LIST_DIR}/include/modules
)
link_directories(${CMAKE_CURRENT_LIST_DIR}/include/libraries)
# Add any user requested libraries


pico_add_extra_outputs(EmbarcaFinal)

