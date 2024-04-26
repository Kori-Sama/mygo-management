#ifndef __HTTP_SERVER_H
#define __HTTP_SERVER_H

#include <string>

namespace http {
    class HttpServer {
    public:
        static HttpServer& instance();

        void init(int port = 9090);

        void run();

        // void route(std::string method, std::string url, HandleFunc handler);

        void use_static(const std::string& path);

    private:
        HttpServer()
        {}

        HttpServer(const HttpServer&) = delete;
        HttpServer& operator=(const HttpServer&) = delete;

        int _port;
    };
}

#endif // __HTTP_SERVER_H