#include "http_server.h"


namespace http {
    HttpServer& HttpServer::instance() {
        static HttpServer server;
        return server;
    }

    void HttpServer::init(int port) {
        _port = port;
    }


}