#pragma once
#include "tcp_server.h"
#include "endpoint.h"


class HttpServer {
private:
    int _port;
public:
    HttpServer()
    {}
    void run();
    void init(int port = 9090, std::string static_root = "www");
    void route(std::string method, std::string url, HandleFunc handler);
};