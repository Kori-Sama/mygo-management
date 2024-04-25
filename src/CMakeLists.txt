aux_source_directory(. DIR_SRCS)

add_subdirectory(./thread_pool)
add_subdirectory(./http)
add_subdirectory(./grpc_client)

add_executable(${TARGET} ${DIR_SRCS})

target_link_libraries(${TARGET} Http GRPC_Client Thread_Pool cJSON)

