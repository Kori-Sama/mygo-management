#ifndef __ENDPOINT_H
#define __ENDPOINT_H

#include "router.h"

namespace http {


    class Endpoint {
    public:
        Endpoint(int sock) :
            _sock(sock)
        {}

        Endpoint(int sock, const std::string& ip, int port) :
            _sock(sock), _ip(ip), _port(port)
        {}

        void handle();
    private:
        bool handle_static(const std::string& path);
        void send_response();
        void build_error_response(HttpCode code);

        HttpRequest _request;
        HttpResponse _response;
        int _sock;
        std::string _ip;
        int _port;
    };
}

#endif // __ENDPOINT_H