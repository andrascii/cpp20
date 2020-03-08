cmake_minimum_required(VERSION 2.8.11)
set(CMAKE_SYSTEM_VERSION 7.0 CACHE STRING INTERNAL FORCE)

set(APP_NAME cpp20)
set(CPP_CONCEPTS cppconcepts)
set(CORO coro)

project(${APP_NAME})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
set(CMAKE_INSTALL_PREFIX)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(${CPP_CONCEPTS})
add_subdirectory(${CORO})