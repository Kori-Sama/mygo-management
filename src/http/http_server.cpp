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
#include "thread_pool.h"

std::vector<ApiHandler*> global_handlers;

void* handle_request(int sock)
{
    auto ep = std::make_unique<EndPoint>(sock);
    ep->read_request();
    if (!ep->is_stop()) {
        ep->register_handlers(global_handlers);
        ep->handle_request();
    }
    if (ep->is_stop()) {
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

        ThreadPool::instance().add_task(std::bind(handle_request, sock));
        int idle = ThreadPool::instance().idle_thread_num();
        INFO("Idle thread num: " + std::to_string(idle));
    }
}

void HttpServer::route(std::string method, std::string url, HandleFunc handler)
{
    auto api_handler = new ApiHandler(method, url, handler);
    global_handlers.push_back(api_handler);
}

void HttpServer::init(int port)
{
    _port = port;
    signal(SIGPIPE, SIG_IGN);
}

