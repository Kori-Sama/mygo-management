#ifndef __HTTP_ROUTER_H
#define __HTTP_ROUTER_H

#include <functional>
#include <string>
#include <vector>
#include "context.h"

namespace http {

    struct RouterInfo {
        std::string method;
        std::string url;
        HandleFunc handler;
    };

    class Router {
    public:
        static Router& instance() {
            static Router router;
            return router;
        }

        void register_router(const std::string& method, const std::string& url, HandleFunc handler) {
            _routers.push_back({ method, url, handler });
        }

        void register_middleware(HandleFunc middleware) {
            _middlewares.push_back(middleware);
        }

        HandleFunc match(const std::string& url, const std::string& method) {
            for (auto& router : _routers) {
                if (router.url == url && router.method == method) {
                    return router.handler;
                }
            }
            return nullptr;
        }

        void pass_middleware(Context& ctx) {
            for (auto& middleware : _middlewares) {
                middleware(ctx);
                if (ctx.is_block()) {
                    return;
                }
            }
        }

    private:
        Router() {}
        Router(const Router&) = delete;
        Router& operator=(const Router&) = delete;

        std::vector<RouterInfo> _routers;
        std::vector<HandleFunc> _middlewares;
    };
}

#endif // __HTTP_ROUTER_H