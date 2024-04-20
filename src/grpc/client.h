#pragma once

#include "transaction.h"


class GrpcClient {
public:
    GrpcClient() {
        const char* port = getenv("GRPC_PORT");
        if (port == NULL) {
            port = "9999";
        }
        std::string address = std::string("localhost:") + std::string(port);
        _transaction_client = TransactionClient(grpc::CreateChannel(address, grpc::InsecureChannelCredentials()));
    }

    std::vector<TransactionResponse> get_all_transactions() {
        return _transaction_client.get_all_transactions();
    }

    TransactionResponse handle_transaction(int id, grpc::TransactionRequest_Action action) {
        return _transaction_client.handle_transaction(id, action);
    }

private:
    TransactionClient _transaction_client;
};

GrpcClient* grpc_instance() {
    static GrpcClient* instance;
    if (instance == nullptr) {
        instance = new GrpcClient();
    }
    return instance;
}
