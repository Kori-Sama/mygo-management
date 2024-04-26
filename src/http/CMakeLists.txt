aux_source_directory(. DIR_LIB_SRCS)

include_directories(${SRC_DIR}/logger 
                    ${SRC_DIR}/thread_pool
                    ${SRC_DIR}/utils)

add_library(Http ${DIR_LIB_SRCS})

target_link_libraries(Http Thread_Pool Logger Utils)