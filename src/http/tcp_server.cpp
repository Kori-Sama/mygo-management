#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "logger.h"
#include "tcp_server.h"

TcpServer* TcpServer::GetInstance(int port)
{
    static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
    if (_svr == nullptr) {
        pthread_mutex_lock(&mtx);
        if (_svr == nullptr) {
            _svr = new TcpServer(port);
            _svr->InitServer();
        }
        pthread_mutex_unlock(&mtx);
    }
    return _svr;
}
void TcpServer::InitServer()
{
    Socket();
    Bind();
    Listen();
}
void TcpServer::Socket()
{
    _listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (_listen_sock < 0) {
        exit(1);
    }
    int opt = 1;
    setsockopt(_listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
}

void TcpServer::Bind()
{
    struct sockaddr_in local;
    memset(&local, 0, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(_port);
    local.sin_addr.s_addr = INADDR_ANY;

    if (bind(_listen_sock, (struct sockaddr*)&local, sizeof(local)) < 0) {
        exit(2);
    }
}

void TcpServer::Listen()
{
    if (listen(_listen_sock, BACKLOG) < 0) {
        exit(3);
    }
}

int TcpServer::Sock()
{
    return _listen_sock;
}

TcpServer::~TcpServer()
{
    if (_listen_sock >= 0) {
        close(_listen_sock);
    }
}