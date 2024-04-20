#include <iostream>
#include "config.h"
#include "http/http_server.h"
int main() {
    Config config;
    config.load_env();
    HttpServer* server = new HttpServer(config.port);
    server->init_server();
    server->run();

    delete server;
}