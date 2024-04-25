#pragma once
#include <stdlib.h>
#include <bits/basic_string.h>

struct Config {
    int port = 9090;
    int grpc_port = 50051;
    int thread_pool_size = 4;

    void load_env() {
        auto env_port = getenv("PORT");
        if (env_port != NULL) {
            port = std::stoi(env_port);
        }

        auto env_grpc_port = getenv("GRPC_PORT");
        if (env_grpc_port != NULL) {
            grpc_port = std::stoi(env_grpc_port);
        }

        auto env_thread_pool_size = getenv("THREAD_POOL_SIZE");
        if (env_thread_pool_size != NULL) {
            thread_pool_size = std::stoi(env_thread_pool_size);
        }
    }
};
