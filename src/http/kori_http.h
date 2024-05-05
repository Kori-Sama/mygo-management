#ifndef __KORI_H
#define __KORI_H

#include "context.h"

namespace http {
    class KoriHttp {
    public:
        /// @brief Run the server
        /// @param port The port to listen
        /// @note This function will block the current thread
        void run(int port);

        /// @brief Register a route
        /// @param method The method of the route supported, should be "GET" or "POST"
        /// @param url The url of the route to serve
        /// @param handler The handler function when the route is matched
        void route(std::string method, std::string url, HandleFunc handler);

        /// @brief Register a static path
        /// @param path The path to serve static files
        void use_static(const std::string& path);

        /// @brief Register a middleware
        /// @param middleware The middleware function to register
        /// @note The middleware function will be called before the route handler
        void middleware(HandleFunc middleware);
    private:
        void print_logo();
    };
}

#endif // __KORI_H