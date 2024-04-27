#ifndef __HTTP_SERVER_H
#define __HTTP_SERVER_H

#include <string>
#include "endpoint.h"

namespace http {
    class HttpServer {
    public:

        void init(int port = 9090);

        void run();

        void route(std::string method, std::string url, HandleFunc handler);

        void use_static(const std::string& path);

        void use(HandleFunc middleware);

    private:
        int _port;
    };
}

#endif // __HTTP_SERVER_H