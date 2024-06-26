#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <memory>
#include <signal.h>
#include "tcp_server.h"
#include "thread_pool.h"
#include "http_server.h"

namespace http {
    HttpServer& HttpServer::instance() {
        static HttpServer server;
        return server;
    }

    void HttpServer::init(int port) {
        _port = port;
        signal(SIGPIPE, SIG_IGN);
    }

    void HttpServer::run() {
        using namespace thread_pool;
        TcpServer::instance().init(_port);
        int listen_sock = TcpServer::instance().Sock();

        while (true) {
            std::cout << "waiting for a request..." << std::endl;

            struct sockaddr_in peer;
            memset(&peer, 0, sizeof(peer));
            socklen_t len = sizeof(peer);
            int sock = accept(listen_sock, (struct sockaddr*)&peer, &len);
            if (sock < 0) {
                continue;
            }


            std::string client_ip = inet_ntoa(peer.sin_addr);
            int client_port = ntohs(peer.sin_port);

            ThreadPool::instance().add_task(
                [sock, client_ip, client_port]() {
                    // Endpoint ep(sock, client_ip, client_port);
                    // ep.handle();
                    auto ep = std::make_unique<Endpoint>(sock, client_ip, client_port);
                    ep->handle();
                    close(sock);
                });
        }
    }

}