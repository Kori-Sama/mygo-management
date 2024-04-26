#ifndef __TCP_SERVER_H
#define __TCP_SERVER_H

namespace http {
    class TcpServer {
    public:
        static TcpServer& instance();

        void init(int port);

        void Socket();

        void Bind();

        void Listen();

        int Sock();

        ~TcpServer();
    private:
        TcpServer() :
            _listen_sock(-1)
        {}

        TcpServer(const TcpServer&) = delete;
        TcpServer& operator=(const TcpServer&) = delete;

        int _port;
        int _listen_sock;
    };
} // namespace http

#endif