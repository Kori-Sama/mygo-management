aux_source_directory(. DIR_LIB_SRCS)

add_library (Http ${DIR_LIB_SRCS})

target_link_libraries(Http Thread_Pool)