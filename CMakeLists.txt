cmake_minimum_required(VERSION 3.28)

project(mygo-management C CXX)

set(CMAKE_CXX_STANDARD 17)

option(ONLY_TEST "Only build test" OFF)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
add_definitions(-DIS_LINUX)
message(STATUS "Configuring on Linux")
else()
message(STATUS "This project is only supported on Linux systems") 
return()
endif()

set(TARGET app)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(THIRD_PARTY_DIR ${PROJECT_ROOT}/third_party)
set(SRC_DIR ${PROJECT_ROOT}/src)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_ROOT}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_ROOT}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_ROOT}/bin/release)

set(CMAKE_DEBUG_POSTFIX "_d") 


include(${PROJECT_ROOT}/cmake/common.cmake)

include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories(${THIRD_PARTY_DIR}/cJSON)
include_directories(${SRC_DIR}/thread_pool)

if(ONLY_TEST)
    enable_testing()
    add_subdirectory(tests)
    return()
endif()
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    enable_testing()
    add_subdirectory(tests)
endif()

add_subdirectory(${THIRD_PARTY_DIR}/cJSON)
add_subdirectory(${SRC_DIR})
