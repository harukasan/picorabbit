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
cmake_minimum_required(VERSION 3.13)


# Set target to Pico 2
set(PICO_BOARD pico2)
set(PICO_PLATFORM rp2350)

include(pico_sdk_import.cmake)

project(picorabbit_dvi C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize Pico SDK
pico_sdk_init()

# Source files
add_executable(picorabbit_dvi
    src/main.c
    src/dvi.c
    src/clock.c
    src/linebuffer.c
)

# Include paths for header files
target_include_directories(picorabbit_dvi PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

# Link required libraries
target_link_libraries(picorabbit_dvi
    pico_stdlib
    pico_multicore
    hardware_pio
    hardware_dma
    hardware_irq
    hardware_clocks
    hardware_vreg
)

# Enable USB and UART output
pico_enable_stdio_usb(picorabbit_dvi 1)
pico_enable_stdio_uart(picorabbit_dvi 1)

# Generate .uf2 file
pico_add_extra_outputs(picorabbit_dvi)
