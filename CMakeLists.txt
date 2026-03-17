cmake_minimum_required(VERSION 3.10)

# Project metadata
project(FLEXISTACK_CICD_TESTER
    VERSION 1.0
    DESCRIPTION "Unit tests for the FlexiStack implementation using Unity framework"
    LANGUAGES C
)

# 1. Set C Standard (C99 is common for STM32/ESP32 firmware)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# 2. Define Folder Paths for clarity
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
set(UNITY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/Unity/src)

# 3. Create the Test Executable
# We include the logic (src), the tests, and the Unity framework implementation
add_executable(run_flexi_tests
    ${SRC_DIR}/stack_operations.c
    ${TEST_DIR}/test_flexistack.c
    ${TEST_DIR}/main.c
    ${UNITY_DIR}/unity.c
)

# 4. Global Include Directories
# This allows you to use #include "unity.h" without relative paths
target_include_directories(run_flexi_tests PRIVATE 
    ${INC_DIR}
    ${UNITY_DIR}
)

# 5. Compiler Flags for Defensive Programming
# We enable warnings to catch common firmware bugs during compilation
if(MSVC)
    target_compile_options(run_flexi_tests PRIVATE /W4)
else()
    target_compile_options(run_flexi_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

# 6. Enable Testing Support
enable_testing()
add_test(NAME FlexiStack_Logic_Test COMMAND run_flexi_tests)