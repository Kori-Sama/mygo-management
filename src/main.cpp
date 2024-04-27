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


    auto jwt_auth = make_jwt_middleware({ "/login" });
    app->middleware(logger_middleware);
    app->middleware(jwt_auth);

    app->route("GET", "/api/transactions", get_all_transactions);
    app->route("GET", "/hello", hello);
    app->route("GET", "/login", [](Context& ctx) {
        ctx.code(HttpCode::OK).send("Login");
        });

    app->run(config.port);
}