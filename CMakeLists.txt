cmake_minimum_required(VERSION 3.16.3)

project(
    "hashtable"
    VERSION "0.1"
    DESCRIPTION "Hash table in C"
)

set(
    PROJECT_SOURCES
    src/hashtable.c
)
add_library("${CMAKE_PROJECT_NAME}" SHARED ${PROJECT_SOURCES})
target_include_directories(
    "${CMAKE_PROJECT_NAME}"
    PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set(
    TEST_SOURCES
    test/test.c
)
add_executable("test" ${TEST_SOURCES})
target_link_libraries("test" "${CMAKE_PROJECT_NAME}")