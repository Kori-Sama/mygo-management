#ifndef __HTTP_PROTOCOL_H
#define __HTTP_PROTOCOL_H

#include <functional>
#include <map>
#include <string>

#include "context.h"

#define HTTP_VERSION "HTTP/1.0"

namespace http {
    constexpr char LINE_END[] = "\r\n";

    typedef std::function<void(Context&)> HandleFunc;

    enum Code {
        OK = 200,
        BAD_REQUEST = 400,
        NOT_FOUND = 404,
        METHOD_NOT_ALLOWED = 405,
        INTERNAL_SERVER_ERROR = 500
    };

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

        static HttpRequest& parser(const std::string& request_str);

        HttpRequest()
            :content_length(0)

        {}
        ~HttpRequest()
        {}
    };


    struct HttpResponse {
        std::string status_line;
        std::string header;
        std::string blank;
        std::string body;
        int status_code;

        static HttpResponse& build(int status_code, std::map<std::string, std::string> header, const std::string& body);

        std::map<std::string, std::string>& header_kv();

        HttpResponse()
            :blank(LINE_END)
            , status_code(Code::OK)
        {}
        ~HttpResponse()
        {}
    };
}

#endif // __HTTP_PROTOCOL_H