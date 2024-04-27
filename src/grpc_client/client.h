#pragma once

#include "transaction.h"

namespace grpc_client {
    class GrpcClient {
    private:
        std::string _port;
    public:
        void init(int port = 50051) {
            _port = std::to_string(port);
            auto channel = grpc::CreateChannel("localhost:" + _port, grpc::InsecureChannelCredentials());
            _transaction_client = TransactionClient(channel);
        }

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

        GrpcClient() {}
        GrpcClient(const GrpcClient&) = delete;
        GrpcClient& operator=(const GrpcClient&) = delete;
    };
}