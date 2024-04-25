cmake_minimum_required(VERSION 3.28)

project(mygo-management)

set(THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

include(${THIRD_PARTY_DIR}/gRPC/common.cmake)
  
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

include_directories(${THIRD_PARTY_DIR}/cJSON)
add_subdirectory(${THIRD_PARTY_DIR}/cJSON)

add_subdirectory(${SRC_DIR})
