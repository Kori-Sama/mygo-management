

aux_source_directory(. DIR_LIB_SRCS)

add_library (Grpc ${DIR_LIB_SRCS})
target_link_libraries(Grpc 
    hw_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})