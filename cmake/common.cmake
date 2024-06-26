find_package(Threads REQUIRED)
option(protobuf_MODULE_COMPATIBLE TRUE)

find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
if(CMAKE_CROSSCOMPILING)
  find_program(_PROTOBUF_PROTOC protoc)
else()
  set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif()

find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP gRPC::grpc++)
if(CMAKE_CROSSCOMPILING)
  find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
  set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif()


set(pbs "transaction")

foreach(pb ${pbs})
  message(STATUS "Generating ${pb}")

  get_filename_component(hw_proto "${PROJECT_ROOT}/protos/${pb}.proto" ABSOLUTE)
  get_filename_component(hw_proto_path "${hw_proto}" PATH)

  set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/${pb}.pb.cc")
  set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/${pb}.pb.h")
  set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/${pb}.grpc.pb.cc")
  set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/${pb}.grpc.pb.h")
  add_custom_command(
        OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
          --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
          -I "${hw_proto_path}"
          --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
          "${hw_proto}"
        DEPENDS "${hw_proto}")
endforeach()

add_library(hw_grpc_proto
  ${hw_grpc_srcs}
  ${hw_grpc_hdrs}
  ${hw_proto_srcs}
  ${hw_proto_hdrs})
target_link_libraries(hw_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})