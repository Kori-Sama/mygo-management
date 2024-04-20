cmake_minimum_required(VERSION 3.28)

project(mygo-management)

aux_source_directory(./src DIR_SRCS)

add_subdirectory(./src/http)

add_executable(${PROJECT_NAME} ${DIR_SRCS})

target_link_libraries(${PROJECT_NAME} Http)