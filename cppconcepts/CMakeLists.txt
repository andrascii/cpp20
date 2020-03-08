cmake_minimum_required(VERSION 3.2)
set(CMAKE_SYSTEM_VERSION 7.0 CACHE STRING INTERNAL FORCE)
set(CMAKE_CXX_STANDARD 20)

project(${CPP_CONCEPTS})

set(APP_SOURCES_DIR src)
set(APP_HEADERS_DIR include)

aux_source_directory(${APP_SOURCES_DIR} APP_SOURCES_LIST)
file(GLOB_RECURSE APP_HEADERS_LIST "include/*.h")

if(MSVC)
    add_definitions(
        /await
    )
endif()

add_executable(
    ${CPP_CONCEPTS}
    ${APP_SOURCES_LIST}
    ${APP_HEADERS_LIST}
)

include_directories(
    ${APP_HEADERS_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(
    ${CPP_CONCEPTS}
)