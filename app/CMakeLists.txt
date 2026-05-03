cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME app)

set(FREERTOS_PORT GCC_ARM_CM7)
set(FREERTOS_HEAP 4)
# set(FREERTOS_CONFIG_FILE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc)
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM
    INTERFACE
    Core/Inc)
target_compile_definitions(freertos_config
    INTERFACE
    projCOVERAGE_TEST=0)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")
# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -u _printf_float")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -u _printf_float")
# Enable CMake support for ASM and C languages
enable_language(C ASM)

add_subdirectory(Middlewares/Third_Party/FreeRTOS/FreeRTOS-Kernel)
add_subdirectory(bsp)
add_subdirectory(Middlewares/Third_Party/FreeRTOS/FreeRTOS-Plus/Source/FreeRTOS-Plus-CLI)
# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search pathsFREERTOS_CONFIG_FILE_DIRECTORY
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)
target_link_libraries(FreeRTOS_CLI PUBLIC
    freertos_kernel)

target_link_libraries(stm32cubemx INTERFACE
    bsp_lib
    FreeRTOS_CLI)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    # Add user defined libraries
    freertos_kernel
)