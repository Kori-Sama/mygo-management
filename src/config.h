#include <stdlib.h>

struct Config {
    int port = 80;

    void load_env() {
        port = std::stoi(getenv("PORT"));
    }
};
