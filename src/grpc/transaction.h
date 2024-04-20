#pragma once
#include <memory>
#include <vector>
#include <grpcpp/grpcpp.h>
#include "transaction.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc::TransactionService;
using grpc::TransactionResponse;
using grpc::TransactionRequest;
using grpc::GetAllTransactionsRequest;

class TransactionClient {
public:
    TransactionClient(std::shared_ptr<Channel> channel)
        :_stub(TransactionService::NewStub(channel))
    {}
    TransactionClient() {}

    std::vector<TransactionResponse> get_all_transactions();
    TransactionResponse handle_transaction(int id, grpc::TransactionRequest_Action action);
private:
    std::unique_ptr<TransactionService::Stub> _stub;
};