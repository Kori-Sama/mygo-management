#include "tool.h"
#include "cJSON.h"

static cJSON* _stringify_transaction(const std::vector<TransactionMessage>& data) {
    cJSON* root = cJSON_CreateArray();
    for (const auto& t : data) {
        cJSON* item = cJSON_CreateObject();
        cJSON_AddNumberToObject(item, "id", t.id());
        cJSON_AddStringToObject(item, "title", t.title().c_str());
        cJSON_AddNumberToObject(item, "value", t.value());
        cJSON_AddStringToObject(item, "description", t.description().c_str());
        cJSON_AddStringToObject(item, "status", grpc::TransactionMessage_Status_Name(t.status()).c_str());
        cJSON_AddNumberToObject(item, "date", t.updated_at().seconds());
        cJSON_AddItemToArray(root, item);
    }
    return root;
}

static cJSON* _stringify_transaction(const TransactionMessage& data) {
    cJSON* root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "id", data.id());
    cJSON_AddStringToObject(root, "title", data.title().c_str());
    cJSON_AddNumberToObject(root, "value", data.value());
    cJSON_AddStringToObject(root, "description", data.description().c_str());
    cJSON_AddStringToObject(root, "status", grpc::TransactionMessage_Status_Name(data.status()).c_str());
    cJSON_AddNumberToObject(root, "date", data.updated_at().seconds());
    return root;
}


std::string stringify_transaction(const std::vector<TransactionMessage> transactions) {
    cJSON* root = _stringify_transaction(transactions);
    std::string json_string = cJSON_Print(root);
    cJSON_Delete(root);
    return json_string;
}

std::string stringify_transaction(const TransactionMessage transaction) {
    cJSON* root = _stringify_transaction(transaction);
    std::string json_string = cJSON_Print(root);
    cJSON_Delete(root);
    return json_string;
}

std::optional<std::unordered_map<std::string, std::string>> json_object_to_map(const char* json) {
    std::unordered_map<std::string, std::string> map;
    cJSON* root = cJSON_Parse(json);
    if (root == nullptr) {
        cJSON_Delete(root);
        return std::nullopt;
    }
    auto item = root->child;
    if (item == nullptr) {
        cJSON_Delete(root);
        return std::nullopt;
    }
    while (item) {
        switch (item->type) {
        case cJSON_String:
            map[item->string] = item->valuestring;
            item = item->next;
            break;
        case cJSON_Number:
            map[item->string] = std::to_string(item->valueint);
            item = item->next;
            break;
        default:
            cJSON_Delete(root);
            return std::nullopt;
            break;
        }
    }
    cJSON_Delete(root);
    return map;
}

std::unordered_map<std::string, grpc::TransactionRequest_Action> action_map = {
    {"get", grpc::TransactionRequest_Action::TransactionRequest_Action_GET},
    {"pass", grpc::TransactionRequest_Action::TransactionRequest_Action_PASS},
    {"reject", grpc::TransactionRequest_Action::TransactionRequest_Action_REJECT},
    {"delete", grpc::TransactionRequest_Action::TransactionRequest_Action_DELETE}
};

grpc::TransactionRequest_Action get_action(std::string action) {
    return action_map[action];
}