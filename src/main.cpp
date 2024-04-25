#include <iostream>
#include "config.h"
#include "routes.h"
#include "thread_pool.h"

int main() {
    auto app = std::make_unique<HttpServer>();
    {
        Config config;
        config.load_env();
        ThreadPool::instance().init(config.thread_pool_size);
        GrpcClient::instance().init(config.grpc_port);
        app->init(config.port, config.static_root);
    }

    app->route("GET", "/api/transactions", get_all_transactions);
    app->route("POST", "/api/transaction", handle_transaction);

    app->run();
}