#pragma once
#include <string>
#include <vector>
#include <unordered_map>

struct HttpRequest {
    std::string request_line;
    std::vector<std::string> request_header;
    std::string blank;
    std::string request_body;


    std::string method;
    std::string uri;
    std::string version;
    std::unordered_map<std::string, std::string> header_kv;
    int content_length;
    std::string path;
    std::string query_string;

    // bool is_cgi;
    HttpRequest()
        :content_length(0)

    {}
    ~HttpRequest()
    {}
};