

aux_source_directory(. DIR_LIB_SRCS)

add_library (GRPC_Client ${DIR_LIB_SRCS})
target_link_libraries(GRPC_Client 
    hw_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})