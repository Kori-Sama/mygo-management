#ifndef __ENDPOINT_H
#define __ENDPOINT_H

#include "router.h"

namespace http {


    class Endpoint {
    public:
        Endpoint(int sock) :
            _sock(sock)
        {}

        void handle();
    private:
        void send_response();
        void build_error_response(HttpCode code);

        HttpRequest _request;
        HttpResponse _response;
        int _sock;
    };
}

#endif // __ENDPOINT_H