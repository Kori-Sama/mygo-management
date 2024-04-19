cmake_minimum_required(VERSION 3.28)

project(mygo-management)

aux_source_directory(./src SRCS)

include_directories(./src)

add_executable(${PROJECT_NAME} ${SRCS})