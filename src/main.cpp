#include <iostream>
#include "config.h"
#include "routes.h"
#include "middleware.h"
#include "thread_pool.h"
#include "http/kori_http.h"
#include "grpc_client/client.h"
using namespace thread_pool;
using namespace grpc_client;
using namespace http;

int main() {
    Config config;
    config.load_env();
    ThreadPool::instance().init(config.thread_pool_size);
    GrpcClient::instance().init(config.grpc_ip, config.grpc_port);

    auto app = std::make_unique<KoriHttp>();

    app->middleware(logger_middleware);
    app->middleware(cors_middleware);
    app->middleware(make_jwt_middleware({ "/login" }));

    // app->use_static("www");

    app->route("GET", "/transactions", get_all_transactions);
    app->route("POST", "/transaction", handle_transactions);

    app->run(config.port);
}