cmake_minimum_required(VERSION 3.2)
set(CMAKE_SYSTEM_VERSION 7.0 CACHE STRING INTERNAL FORCE)
set(CMAKE_CXX_STANDARD 20)

project(${STANDARDLIB})

set(SOURCES_DIR src)
set(HEADERS_DIR include)

aux_source_directory(${SOURCES_DIR} SOURCES_LIST)
file(GLOB_RECURSE HEADERS_LIST "include/*.h")

if(MSVC)
    add_definitions(
        /await
    )
endif()

add_executable(
    ${STANDARDLIB}
    ${SOURCES_LIST}
    ${HEADERS_LIST}
)

include_directories(
    ${HEADERS_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(
    ${STANDARDLIB}
)