#include "kori.h"
#include "router.h"
#include "http_server.h"

namespace http {
    void KoriHttp::run(int port) {
        print_logo();
        HttpServer::instance().init(port);
        HttpServer::instance().run();
    }

    void KoriHttp::route(std::string method, std::string url, HandleFunc handler) {
        Router::instance().register_router(method, url, handler);
    }

    void KoriHttp::use_static(const std::string& path) {
        Router::instance().register_static(path);
    }

    void KoriHttp::middleware(HandleFunc middleware) {
        Router::instance().register_middleware(middleware);
    }

    void KoriHttp::print_logo() {
        auto logo = R"(
  _______ ____   _____ ______   _   _           _____ _    _ _____ 
 |__   __/ __ \ / ____|  ____| | \ | |   /\    / ____| |  | |_   _|
    | | | |  | | |  __| |__    |  \| |  /  \  | (___ | |__| | | |  
    | | | |  | | | |_ |  __|   | . ` | / /\ \  \___ \|  __  | | |  
    | | | |__| | |__| | |____  | |\  |/ ____ \ ____) | |  | |_| |_ 
  __|_|__\____/ \_____|______| |_| \_/_/ ___\_\_____/|_|  |_|_____|
 |__   __/ __ \ / ____|  ____|     /\   |  __ \|_   _|             
    | | | |  | | |  __| |__       /  \  | |__) | | |               
    | | | |  | | | |_ |  __|     / /\ \ |  _  /  | |               
    | | | |__| | |__| | |____   / ____ \| | \ \ _| |_              
    |_|  \____/ \_____|______| /_/    \_\_|  \_\_____|             
        )";
        printf("%s\n", logo);
    }

}