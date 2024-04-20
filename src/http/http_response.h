#pragma once
#include <string>
#include <vector>
#include "status_code.h"
#define LINE_END "\r\n"

struct HttpResponse {
    std::string status_line;
    std::vector<std::string> response_header;
    std::string blank;
    std::string response_body;


    int status_code;
    int fd;
    int size;
    std::string suffix;
    HttpResponse()
        :blank(LINE_END)
        , status_code(Code::OK)
        , fd(-1)
        , size(0)
    {}
    ~HttpResponse()
    {}
};