cmake_minimum_required(VERSION 3.28)

project(mygo-management C CXX)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_definitions(-DIS_LINUX)
    message(STATUS "Configuring on Linux")
else()
    message(STATUS "This project is only supported on Linux systems") 
    return()
endif()

set(TARGET app)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin/release)

set(CMAKE_DEBUG_POSTFIX "_d") 

set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/common.cmake)
  
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories(${THIRD_PARTY_DIR}/cJSON)
include_directories(${SRC_DIR}/thread_pool)

add_subdirectory(${THIRD_PARTY_DIR}/cJSON)
add_subdirectory(${SRC_DIR})
add_subdirectory(tests)