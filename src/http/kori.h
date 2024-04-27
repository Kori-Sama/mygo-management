#ifndef __KORI_H
#define __KORI_H

#include "context.h"

namespace http {

    class KoriHttp {
    public:
        void run(int port);

        void route(std::string method, std::string url, HandleFunc handler);

        void use_static(const std::string& path);

        void use(HandleFunc middleware);
    };

}

#endif // __KORI_H