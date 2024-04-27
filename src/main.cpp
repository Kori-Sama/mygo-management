#include <iostream>
#include "config.h"
#include "routes.h"
#include "middleware.h"
#include "thread_pool.h"
#include "http/kori.h"
#include "grpc_client/client.h"
using namespace thread_pool;
using namespace grpc_client;
using namespace http;

int main() {

    Config config;
    config.load_env();
    ThreadPool::instance().init(config.thread_pool_size);
    GrpcClient::instance().init(config.grpc_port);

    auto app = std::make_unique<KoriHttp>();

    app->use(middleware1);

    app->route("GET", "/api/transactions", get_all_transactions);
    app->route("GET", "/hello", hello);
    // app->route("POST", "/api/transaction", handle_transaction);

    app->run(config.port);
}