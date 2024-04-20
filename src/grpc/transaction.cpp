#include "transaction.h"

std::vector<TransactionResponse> TransactionClient::get_all_transactions()
{
    GetAllTransactionsRequest request;
    ClientContext context;
    auto reader = _stub->GetAllTransactions(&context, request);
    std::vector<TransactionResponse> transactions;
    TransactionResponse response;
    while (reader->Read(&response)) {
        transactions.push_back(response);
    }

    Status status = reader->Finish();
    if (!status.ok()) {
        printf("TransactionClient::get_all_transactions rpc failed\n");
    }
    return transactions;
}

TransactionResponse TransactionClient::handle_transaction(int id, grpc::TransactionRequest_Action action)
{
    ClientContext context;

    TransactionRequest request;
    request.set_id(id);
    request.set_action(action);

    TransactionResponse response;

    Status status = _stub->HandleTransaction(&context, request, &response);

    if (!status.ok()) {
        printf("TransactionClient::handle_transaction rpc failed\n");
    }
    return response;
}
