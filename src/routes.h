// #pragma once
// #include "http/http_server.h"
// #include "utils.h"

// void get_all_transactions(Context& ctx) {
//     auto transactions = GrpcClient::instance().get_all_transactions();
//     ctx.status(Code::OK);
//     ctx.body(stringify_transaction(transactions));
// }

// void handle_transaction(Context& ctx) {
//     auto id = ctx.query("id");
//     auto action = ctx.query("action");

//     auto t = GrpcClient::instance().handle_transaction(std::stoi(id), get_action(action));
//     ctx.status(Code::OK);
//     ctx.body(stringify_transaction(t));
// }
