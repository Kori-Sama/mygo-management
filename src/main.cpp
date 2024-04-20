#include <iostream>
#include "config.h"
#include "routes.h"

int main() {
    Config config;
    config.load_env();
    HttpServer* app = new HttpServer(config.port);

    app->init_server();

    app->route("GET", "/api/transactions", get_all_transactions);
    app->route("POST", "/api/transaction", handle_transaction);

    app->run();

    delete app;
}