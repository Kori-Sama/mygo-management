#pragma once
#include <stdlib.h>

struct Config {
    int port = 8080;

    void load_env() {
        auto env_port = getenv("PORT");
        if (env_port != NULL) {
            port = std::stoi(env_port);
        }
    }
};
