#ifndef __HTTP_SERVER_H
#define __HTTP_SERVER_H

#include <string>
#include "endpoint.h"

namespace http {
    class HttpServer {
    public:
        static HttpServer& instance();

        void init(int port = 9090);

        void run();

    private:
        HttpServer() {}

        HttpServer(const HttpServer&) = delete;
        HttpServer& operator=(const HttpServer&) = delete;

        int _port;
    };
}

#endif // __HTTP_SERVER_H