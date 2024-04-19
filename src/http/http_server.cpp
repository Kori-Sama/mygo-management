#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <memory>
#include "logger.h"
#include "http_server.h"
#include "endpoint.h"

void* handle_request(void* arg)
{
    int sock = *(int*)arg;

    auto ep = std::make_unique<EndPoint>(sock);
    ep->recvHttpRequest();    //读取请求
    ep->handlerHttpRequest(); //处理请求
    ep->buildHttpResponse();  //构建响应
    ep->sendHttpResponse();   //发送响应

    close(sock); //关闭与该客户端建立的套接字

    return nullptr;
}

void HttpServer::run()
{
    TcpServer* tcp = TcpServer::GetInstance(_port);
    int listen_sock = tcp->Sock();

    INFO("Server is running on port: " + std::to_string(_port) + "...");

    while (true) {
        INFO("Wait for a new connection...");

        struct sockaddr_in peer;
        memset(&peer, 0, sizeof(peer));
        socklen_t len = sizeof(peer);
        int sock = accept(listen_sock, (struct sockaddr*)&peer, &len); //获取新连接
        if (sock < 0) {
            continue; //获取失败，继续获取
        }

        //打印客户端相关信息
        std::string client_ip = inet_ntoa(peer.sin_addr);
        int client_port = ntohs(peer.sin_port);
        INFO("Get a new connection: [" + client_ip + ":" + std::to_string(client_port) + "]");

        //创建新线程处理新连接发起的HTTP请求
        int* p = new int(sock);
        pthread_t tid;
        pthread_create(&tid, nullptr, handle_request, (void*)p);
        pthread_detach(tid); //线程分离
    }
}
