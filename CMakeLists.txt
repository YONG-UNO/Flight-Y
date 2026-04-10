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
set(CMAKE_PROJECT_NAME Flight-Y)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)
# 启用C++支持(适配.cpp文件)
enable_language(CXX)
# 添加C++标准(嵌入式常用C++17/C++11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)           # 禁用编译器特有拓展,提升代码可移植性

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME}
#        UserLib/Module/Device/optical_flow/optical_flow.c
#        UserLib/Module/Device/Infrared_ranging/Infrared_ranging.c
#        UserLib/BSP/Soft_IIC/soft_iic2.c
#        UserLib/Module/Device/LED/LED.c
#        UserLib/Module/Device/MPU6050/MPU6050.c
#        UserLib/Module/Algorithm/Kalman/kalman.c
#        #        UserLib/Module/Algorithm/MPU_Integration/MPU_Integration.c
#        UserLib/Module/Task/Integration/integration.c
#        UserLib/Module/Device/MPU6050/MahonyAHRS.c
#        UserLib/Module/Task/Control_Pitch/Control_Pitch.c
        UserLib/Module/Algorithm/PID/PID.cpp
)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
#        ${CMAKE_SOURCE_DIR}/UserLib/Module/Device/optical_flow
#        ${CMAKE_SOURCE_DIR}/UserLib/Module/Device/infrared_ranging
#        ${CMAKE_SOURCE_DIR}/UserLib/BSP/Soft_IIC
#        ${CMAKE_SOURCE_DIR}/UserLib/Module/Device/LED
#        ${CMAKE_SOURCE_DIR}/UserLib/Module/Device/MPU6050
#        ${CMAKE_SOURCE_DIR}/UserLib/Module/Algorithm/Kalman
#        ${CMAKE_SOURCE_DIR}/UserLib/Module/Algorithm/PID
#        ${CMAKE_SOURCE_DIR}/UserLib/Module/Algorithm/MPU_Integration
#        ${CMAKE_SOURCE_DIR}/UserLib/Module/Task/Integration
#        ${CMAKE_SOURCE_DIR}/UserLib/Module/Task/Control
        ${CMAKE_SOURCE_DIR}/UserLib/Module/Algorithm/PID
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx

    # Add user defined libraries
)
