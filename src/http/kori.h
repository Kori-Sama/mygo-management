#ifndef __KORI_H
#define __KORI_H

#include "context.h"

namespace http {

    class KoriHttp {
    public:
        void run(int port);

        void route(std::string method, std::string url, HandleFunc handler);

        void use_static(const std::string& path);

        void middleware(HandleFunc middleware);
    private:
        void print_logo();
    };

}

#endif // __KORI_H