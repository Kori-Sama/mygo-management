// #pragma once
// #include "http/http_server.h"
// #include "utils.h"

// void get_all_transactions(Context& ctx) {
//     auto transactions = GrpcClient::instance().get_all_transactions();
//     ctx.status(HttpCode::OK);
//     ctx.body(stringify_transaction(transactions));
// }

// void handle_transaction(Context& ctx) {
//     auto id = ctx.query("id");
//     auto action = ctx.query("action");

//     auto t = GrpcClient::instance().handle_transaction(std::stoi(id), get_action(action));
//     ctx.status(HttpCode::OK);
//     ctx.body(stringify_transaction(t));
// }

#ifndef __ROUTES_H
#define __ROUTES_H

#include "http/http_server.h"
#include "grpc_client/client.h"
#include "tool.h"

using namespace http;

void get_all_transactions(Context& ctx) {
    auto transactions = grpc_client::GrpcClient::instance().get_all_transactions();
    ctx.code(HttpCode::OK)
        .send(stringify_transaction(transactions).c_str());
}

void hello(Context& ctx) {
    auto name = ctx.query("name");
    ctx.code(HttpCode::OK).html(std::string("</h1>Hello " + name + "</h1>").c_str());
}

#endif // __ROUTES_H