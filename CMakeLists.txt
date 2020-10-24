cmake_minimum_required (VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(APP_NAME cpp20)
set(CPP_CONCEPTS cppconcepts)
set(CORO coro)
set(STANDARDLIB standardlib)

project(${APP_NAME} LANGUAGES C CXX)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
set(CMAKE_INSTALL_PREFIX)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(${CPP_CONCEPTS})
add_subdirectory(${CORO})
add_subdirectory(${STANDARDLIB})