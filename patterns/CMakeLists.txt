cmake_minimum_required(VERSION 3.2)
set(CMAKE_SYSTEM_VERSION 7.0 CACHE STRING INTERNAL FORCE)
set(CMAKE_CXX_STANDARD 20)

set(THIS_TARGET ${PATTERNS})

project(${THIS_TARGET})

set(SOURCES_DIR src)
set(HEADERS_DIR include)

aux_source_directory(${SOURCES_DIR} SOURCES_LIST)
file(GLOB_RECURSE HEADERS_LIST "include/*.h")

if(MSVC)
    add_definitions(
        /await
    )
endif()

find_package(date CONFIG REQUIRED)

add_executable(
    ${THIS_TARGET}
    ${SOURCES_LIST}
    ${HEADERS_LIST}
)

include_directories(
    ${HEADERS_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(
    ${THIS_TARGET}
    PRIVATE
    date::date
    date::date-tz
)
