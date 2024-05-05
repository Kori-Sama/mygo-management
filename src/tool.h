#ifndef __TOOL_H
#define __TOOL_H

#include <string>
#include <vector>
#include "grpc_client/client.h"

std::string stringify_transaction(const std::vector<TransactionMessage> transactions);
std::string stringify_transaction(const TransactionMessage transaction);

std::optional<std::unordered_map<std::string, std::string>> json_object_to_map(const char* json);

grpc::TransactionRequest_Action get_action(std::string action);
#endif // __TOOL_H