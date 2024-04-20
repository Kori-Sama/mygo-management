#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <memory>
#include <signal.h>
#include "logger.h"
#include "http_server.h"

void* handle_request(void* arg)
{
    int sock = *(int*)arg;

    auto ep = std::make_unique<EndPoint>(sock);
    ep->read_request();
    if (!ep->is_stop()) {
        ep->handle_request();
        ep->build_response();
        ep->send_response();
    } else {
        ERROR(ep->get_error());
    }
    close(sock);
    auto msgs = ep->get_session_msg();
    INFO(msgs[0]);
    INFO(msgs[1]);
    return nullptr;
}

void HttpServer::run()
{
    auto tcp = std::make_unique<TcpServer>(_port);
    tcp->InitServer();
    int listen_sock = tcp->Sock();

    INFO("Server is running on port: " + std::to_string(_port) + "...");

    while (true) {
        INFO("Wait for a new connection...");

        struct sockaddr_in peer;
        memset(&peer, 0, sizeof(peer));
        socklen_t len = sizeof(peer);
        int sock = accept(listen_sock, (struct sockaddr*)&peer, &len);
        if (sock < 0) {
            continue;
        }

        std::string client_ip = inet_ntoa(peer.sin_addr);
        int client_port = ntohs(peer.sin_port);
        INFO("Get a new connection: [" + client_ip + ":" + std::to_string(client_port) + "]");

        int* p = new int(sock);
        pthread_t tid;
        pthread_create(&tid, nullptr, handle_request, (void*)p);
        pthread_detach(tid);
    }
}

void HttpServer::init_server()
{
    signal(SIGPIPE, SIG_IGN);
}

