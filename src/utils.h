#pragma once
#include <string>
#include <vector>
#include "grpc_client/client.h"

std::string stringify_transaction(std::vector<TransactionMessage> transactions);
std::string stringify_transaction(TransactionMessage transaction);

grpc::TransactionRequest_Action get_action(std::string action);