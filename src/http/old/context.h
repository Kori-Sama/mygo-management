#pragma once
#include "http_request.h"
#include "http_response.h"
#include "utils.h"

class Context {
private:
    HttpRequest* _request;
    HttpResponse* _response;
    bool _is_setted_content_type;
public:
    Context(HttpRequest* req, HttpResponse* resp)
        :_request(req)
        , _response(resp)
        , _is_setted_content_type(false)
    {}

    ~Context() {
        if (!_is_setted_content_type) {
            content_type("application/json");
        }
    }

    HttpRequest* request() {
        return _request;
    }

    void status(int code) {
        _response->status_code = code;
    }
    void header(std::string key, std::string value) {
        _response->response_header.push_back(key + ": " + value + LINE_END);

    }

    void body(std::string body) {
        _response->response_body = body;
    }
    void content_type(std::string type) {
        header("Content-Type", type);
        _is_setted_content_type = true;
    }

    std::string query(std::string key) {
        auto querys = utils::split(_request->query_string, "&");
        for (auto& query : querys) {
            auto kv = utils::split(query, "=");
            if (kv[0] == key) {
                return kv[1];
            }
        }
        return "";
    }
};