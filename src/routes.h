#pragma once
#include <vector>
#include "http/http_server.h"
#include "grpc/client.h"

std::string to_json(std::vector<TransactionResponse> data) {
    std::string res = "[";
    for (size_t i = 0;i < data.size();i++) {
        auto t = data[i].transaction();
        res += "{";
        res += "\"id\":" + std::to_string(t.id()) + ",";
        res += "\"title\":\"" + t.title() + "\",";
        res += "\"value\":\"" + std::to_string(t.value()) + "\",";
        res += "\"description\":\"" + t.description() + "\",";
        res += "\"status\":\"" + grpc::TransactionMessage_Status_Name(t.status()) + "\",";
        res += "\"date\":\"" + std::to_string(t.updated_at().seconds()) + "\"";
        if (i == data.size() - 1) {
            res += "}";
        } else {
            res += "},";
        }
    }
    res += "]";
    return res;
}

std::string to_json(TransactionResponse data) {
    std::string res;

    auto t = data.transaction();
    res += "{";
    res += "\"id\":" + std::to_string(t.id()) + ",";
    res += "\"title\":\"" + t.title() + "\",";
    res += "\"value\":\"" + std::to_string(t.value()) + "\",";
    res += "\"description\":\"" + t.description() + "\",";
    res += "\"status\":\"" + grpc::TransactionMessage_Status_Name(t.status()) + "\",";
    res += "\"date\":\"" + std::to_string(t.updated_at().seconds()) + "\"";
    res += "}";

    return res;
}


void get_all_transactions(Context& ctx) {
    // std::string test = R"([{"id":1,"title":"Test 1","value":"100","description":"Test 1 description","status":"passed","date":"4/20/2024"},{"id":2,"title":"Test 2","value":"200","description":"Test 2 description","status":"reject","date":"4/20/2024"},{"id":3,"title":"Test 3","value":"300","description":"Test 3 description","status":"censor","date":"4/20/2024"},{"id":4,"title":"Test 4","value":"400","description":"Test 4 description","status":"draft","date":"4/20/2024"},{"id":5,"title":"Test 5","value":"500","description":"Test 5 description","status":"passed","date":"4/20/2024"},{"id":6,"title":"Test 6","value":"600","description":"Test 6 description","status":"reject","date":"4/20/2024"},{"id":7,"title":"Test 7","value":"700","description":"Test 7 description","status":"censor","date":"4/20/2024"},{"id":8,"title":"Test 8","value":"800","description":"Test 8 description","status":"draft","date":"4/20/2024"},{"id":9,"title":"Test 9","value":"900","description":"Test 9 description","status":"passed","date":"4/20/2024"},{"id":10,"title":"Test 10","value":"1000","description":"Test 10 description","status":"reject","date":"1/1/2022"},{"id":11,"title":"Test 11","value":"1100","description":"Test 11 description","status":"censor","date":"1/1/2022"},{"id":12,"title":"Test 12","value":"1200","description":"Test 12 description","status":"draft","date":"1/1/2022"},{"id":13,"title":"Test 13","value":"1300","description":"Test 13 description","status":"passed","date":"1/1/2022"},{"id":14,"title":"Test 14","value":"1400","description":"Test 14 description","status":"reject","date":"1/1/2022"},{"id":15,"title":"Test 15","value":"1500","description":"Test 15 description","status":"censor","date":"1/1/2022"},{"id":16,"title":"Test 16","value":"1600","description":"Test 16 description","status":"draft","date":"1/1/2022"},{"id":17,"title":"Test 17","value":"1700","description":"Test 17 description","status":"passed","date":"1/1/2022"},{"id":18,"title":"Test 18","value":"1800","description":"Test 18 description","status":"reject","date":"1/1/2022"},{"id":19,"title":"Test 19","value":"1900","description":"Test 19 description","status":"censor","date":"1/1/2022"},{"id":20,"title":"Test 20","value":"2000","description":"Test 20 description","status":"draft","date":"1/1/2022"},{"id":21,"title":"Test 21","value":"2100","description":"Test 21 description","status":"passed","date":"1/1/2022"},{"id":22,"title":"Test 22","value":"2200","description":"Test 22 description","status":"reject","date":"1/1/2022"},{"id":23,"title":"Test 23","value":"2300","description":"Test 23 description","status":"censor","date":"1/1/2022"},{"id":24,"title":"Test 24","value":"2400","description":"Test 24 description","status":"draft","date":"1/1/2022"},{"id":25,"title":"Test 25","value":"2500","description":"Test 25 description","status":"passed","date":"1/1/2022"},{"id":26,"title":"Test 26","value":"2600","description":"Test 26 description","status":"reject","date":"1/1/2022"},{"id":27,"title":"Test 27","value":"2700","description":"Test 27 description","status":"censor","date":"1/1/2022"},{"id":28,"title":"Test 28","value":"2800","description":"Test 28 description","status":"draft","date":"1/1/2022"},{"id":29,"title":"Test 29","value":"2900","description":"Test 29 description","status":"passed","date":"1/1/2022"}])";
    // auto transactions = GrpcClient::instance()->get_all_transactions();

    auto transactions = grpc_instance()->get_all_transactions();
    ctx.status(Code::OK);
    ctx.body(to_json(transactions));
}

grpc::TransactionRequest_Action get_action(std::string action) {
    if (action == "get") {
        return grpc::TransactionRequest_Action::TransactionRequest_Action_GET;
    } else if (action == "pass") {
        return grpc::TransactionRequest_Action::TransactionRequest_Action_PASS;
    } else if (action == "reject") {
        return grpc::TransactionRequest_Action::TransactionRequest_Action_REJECT;
    } else if (action == "delete") {
        return grpc::TransactionRequest_Action::TransactionRequest_Action_DELETE;
    } else {
        return grpc::TransactionRequest_Action::TransactionRequest_Action_GET;
    }
}

void handle_transaction(Context& ctx) {
    auto id = ctx.query("id");
    auto action = ctx.query("action");

    auto t = grpc_instance()->handle_transaction(std::stoi(id), get_action(action));

    ctx.status(Code::OK);
    ctx.body(to_json(t));
}
