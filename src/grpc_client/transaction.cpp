#include "transaction.h"

namespace grpc_client {
    std::vector<TransactionMessage> TransactionClient::get_all_transactions() {
        GetAllTransactionsRequest request;
        ClientContext context;
        auto reader = _stub->GetAllTransactions(&context, request);
        std::vector<TransactionMessage> transactions;
        TransactionResponse response;
        while (reader->Read(&response)) {
            auto msg = response.transaction();
            transactions.push_back(msg);
        }

        Status status = reader->Finish();
        if (!status.ok()) {
            printf("TransactionClient::get_all_transactions rpc failed\n");
        }

        std::cout << "[GRPC] << Get all transactions" << std::endl;

        return transactions;
    }

    TransactionMessage TransactionClient::handle_transaction(int id, grpc::TransactionRequest_Action action) {
        ClientContext context;

        TransactionRequest request;
        request.set_id(id);
        request.set_action(action);

        TransactionResponse response;

        Status status = _stub->HandleTransaction(&context, request, &response);

        if (!status.ok()) {
            printf("TransactionClient::handle_transaction rpc failed\n");
        }

        std::cout << "[GRPC] << Handle transaction" << std::endl;
        return response.transaction();
    }
}