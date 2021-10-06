cmake_minimum_required (VERSION 3.14)

set(APP_NAME cpp)
project(${APP_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CPP_CONCEPTS cppconcepts)
set(COROUTINES coroutines)
set(STANDARDLIB standardlib)
set(PATTERNS patterns)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
set(CMAKE_INSTALL_PREFIX)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(${CPP_CONCEPTS})
add_subdirectory(${COROUTINES})
add_subdirectory(${STANDARDLIB})
add_subdirectory(${PATTERNS})