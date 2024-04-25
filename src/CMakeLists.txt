aux_source_directory(. DIR_SRCS)

add_executable(${PROJECT_NAME} ${DIR_SRCS})

add_subdirectory(./http)
add_subdirectory(./grpc_client)
target_link_libraries(${PROJECT_NAME} Http Grpc cJSON)

