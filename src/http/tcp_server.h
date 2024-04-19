#define BACKLOG 5

class TcpServer {
private:
    int _port;
    int _listen_sock;
    static TcpServer* _svr;
private:
    TcpServer(int port)
        :_port(port)
        , _listen_sock(-1)
    {}
    TcpServer(const TcpServer&) = delete;
    TcpServer* operator=(const TcpServer&) = delete;
public:
    static TcpServer* GetInstance(int port);

    void InitServer();

    void Socket();

    void Bind();

    void Listen();

    int Sock();

    ~TcpServer();
};