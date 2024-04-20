#pragma once
#define BACKLOG 5

class TcpServer {
private:
    int _port;
    int _listen_sock;
public:
    TcpServer(int port)
        :_port(port)
        , _listen_sock(-1)
    {}

    void InitServer();

    void Socket();

    void Bind();

    void Listen();

    int Sock();

    ~TcpServer();
};