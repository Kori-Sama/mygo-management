#ifndef __HTTP_PROTOCOL_H
#define __HTTP_PROTOCOL_H

#include <functional>
#include <map>
#include <string>

#include "http_code.h"

#define HTTP_VERSION "HTTP/1.1"
#define LINE_END "\r\n"

namespace http {
    struct HttpRequest {
        std::string request_line;
        std::string header;
        std::string blank;
        std::string body;


        std::string method;
        std::string uri;
        std::string version;
        std::unordered_map<std::string, std::string> header_kv;
        int content_length;
        std::string path;
        std::string query_string;
        std::unordered_map<std::string, std::string> query_kv;

        bool parse(const std::string& request_str);

        HttpRequest()
            :content_length(0) {};

        ~HttpRequest()
        {}
    };


    struct HttpResponse {
        std::string status_line;
        std::string header;
        std::string blank;
        std::string body;
        HttpCode status_code;
        std::unordered_map<std::string, std::string> header_kv;

        void build();

        void set_header(const char* key, const char* value);

        std::string to_string();
        std::string to_string_without_body();

        HttpResponse()
            :blank(LINE_END)
            , status_code(HttpCode::OK)
        {}

        ~HttpResponse()
        {}
    };
}

#endif // __HTTP_PROTOCOL_H