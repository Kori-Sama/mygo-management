#ifndef __CONFIG_H
#define __CONFIG_H

#include <stdlib.h>
#include <bits/basic_string.h>

struct Config {
    // The port to listen
    int port = 9090;

    // The IP address of the gRPC server
    std::string grpc_ip = "localhost";

    // The port of the gRPC server
    int grpc_port = 50051;

    // The size of the thread pool
    int thread_pool_size = 4;

    // The root path of the static files
    std::string static_root = "www";

    /// @brief Load the environment variables
    /// @note The environment variables will be set a default value if not found
    void load_env() {
        auto env_port = getenv("PORT");
        auto env_grpc_ip = getenv("GRPC_IP");
        auto env_grpc_port = getenv("GRPC_PORT");
        auto env_thread_pool_size = getenv("THREAD_POOL_SIZE");
        auto env_www_root = getenv("STATIC_ROOT");

        if (env_port != NULL)
            port = std::stoi(env_port);
        if (env_grpc_ip != NULL)
            grpc_ip = std::string(env_grpc_ip);
        if (env_grpc_port != NULL)
            grpc_port = std::stoi(env_grpc_port);
        if (env_thread_pool_size != NULL)
            thread_pool_size = std::stoi(env_thread_pool_size);
        if (env_www_root != NULL)
            static_root = env_www_root;
    }
};

#endif // __CONFIG_H