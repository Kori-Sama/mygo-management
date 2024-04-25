#pragma once

#include "transaction.h"


class GrpcClient {
public:
    std::vector<TransactionMessage> get_all_transactions() {
        return _transaction_client.get_all_transactions();
    }

    TransactionMessage handle_transaction(int id, grpc::TransactionRequest_Action action) {
        return _transaction_client.handle_transaction(id, action);
    }

    static GrpcClient& instance() {
        static GrpcClient instance;
        return instance;
    }

private:
    TransactionClient _transaction_client;

    GrpcClient() {
        const char* port = getenv("GRPC_PORT");
        if (port == NULL) {
            port = "50051";
        }
        std::string address = std::string("localhost:") + std::string(port);
        _transaction_client = TransactionClient(grpc::CreateChannel(address, grpc::InsecureChannelCredentials()));
    }
};

// GrpcClient* grpc_instance() {
//     static GrpcClient* instance;
//     if (instance == nullptr) {
//         instance = new GrpcClient();
//     }
//     return instance;
// }
