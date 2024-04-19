#include "tcp_server.h"

class HttpServer {
private:
    int _port;
public:
    HttpServer(int port)
        :_port(port)
    {}

    void run();
};