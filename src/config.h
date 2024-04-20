#pragma once
#include <stdlib.h>

struct Config {
    int port = 9090;

    void load_env() {
        auto env_port = getenv("PORT");
        if (env_port != NULL) {
            port = std::stoi(env_port);
        }
    }
};
