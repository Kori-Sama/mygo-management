#ifndef __CONFIG_H
#define __CONFIG_H

#include <stdlib.h>
#include <bits/basic_string.h>

struct Config {
    int port = 9090;
    int grpc_port = 50051;
    int thread_pool_size = 4;
    std::string static_root = "www";

    void load_env() {
        auto env_port = getenv("PORT");
        auto env_grpc_port = getenv("GRPC_PORT");
        auto env_thread_pool_size = getenv("THREAD_POOL_SIZE");
        auto env_www_root = getenv("STATIC_ROOT");

        if (env_port != NULL)
            port = std::stoi(env_port);
        if (env_grpc_port != NULL)
            grpc_port = std::stoi(env_grpc_port);
        if (env_thread_pool_size != NULL)
            thread_pool_size = std::stoi(env_thread_pool_size);
        if (env_www_root != NULL)
            static_root = env_www_root;
    }
};

#endif // __CONFIG_H