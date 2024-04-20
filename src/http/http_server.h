#pragma once
#include "tcp_server.h"
#include "endpoint.h"

class HttpServer {
private:
    int _port;
public:
    HttpServer(int port)
        :_port(port)
    {}
    void run();
    void init_server();
};