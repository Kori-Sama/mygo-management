cmake_minimum_required(VERSION 3.28)

project(mygo-management)

set(CMAKE_CXX_STANDARD 17)

set(TARGET app)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_DEBUG_POSTFIX "_d")
set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "_d")

set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

include(${THIRD_PARTY_DIR}/gRPC/common.cmake)
  
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
include_directories(${THIRD_PARTY_DIR}/cJSON)
include_directories(${SRC_DIR}/thread_pool)

add_subdirectory(${THIRD_PARTY_DIR}/cJSON)
add_subdirectory(${SRC_DIR})
