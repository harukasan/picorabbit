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

set(BUILD_CONFIG picorabbit-mruby-cortex-m33)

# Build MRuby
add_custom_target(build_mruby
    COMMAND rake MRUBY_CONFIG=${CMAKE_CURRENT_SOURCE_DIR}/build_config/${BUILD_CONFIG}.rb
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor/mruby
    COMMENT "Building mruby..."
)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/main_task.c
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/mrblib/main_task.rb
    COMMAND ${CMAKE_CURRENT_LIST_DIR}/vendor/mruby/bin/mrbc -Bmain_task -o ${CMAKE_BINARY_DIR}/main_task.c ${CMAKE_CURRENT_SOURCE_DIR}/mrblib/main_task.rb
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/mrblib
    COMMENT "Building main_task.c from main_task.rb..."
)

# Add MRuby library
add_library(mruby STATIC IMPORTED)
set_target_properties(mruby PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/vendor/mruby/build/${BUILD_CONFIG}/lib/libmruby.a
)

# Source files
add_executable(picorabbit_dvi
    src/main.c
    src/dvi.c
    src/clock.c
    src/line_buffer.c
    src/draw_text.c
    mrbgems/mruby-pico-memory-usage/ports/rp2350/memory_usage.c
    ${CMAKE_BINARY_DIR}/main_task.c
)

# Add dependency to ensure MRuby is built before the main target
add_dependencies(picorabbit_dvi build_mruby)

# Include paths for header files
target_include_directories(picorabbit_dvi PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/mruby/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/mruby/build/${BUILD_CONFIG}/include
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
    mruby
)

# Enable USB and UART output
pico_enable_stdio_usb(picorabbit_dvi 1)
pico_enable_stdio_uart(picorabbit_dvi 1)

# Generate .uf2 file
pico_add_extra_outputs(picorabbit_dvi)

string(APPEND CMAKE_EXE_LINKER_FLAGS "-Wl,--print-memory-usage")
