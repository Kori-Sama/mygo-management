#include "kori.h"
#include "router.h"
#include "http_server.h"

namespace http {
    void KoriHttp::run(int port) {
        HttpServer::instance().init(port);
        HttpServer::instance().run();
    }

    void KoriHttp::route(std::string method, std::string url, HandleFunc handler) {
        Router::instance().register_router(method, url, handler);
    }

    void KoriHttp::use_static(const std::string& path) {
    }

    void KoriHttp::use(HandleFunc middleware) {
        Router::instance().register_middleware(middleware);
    }

}