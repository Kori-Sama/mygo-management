#include "http_request.h"
#include "http_response.h"

class EndPoint {
private:
    int _sock;                   //通信的套接字
    HttpRequest _http_request;   //HTTP请求
    HttpResponse _http_response; //HTTP响应
public:
    EndPoint(int sock)
        :_sock(sock)
    {}
    //读取请求
    void recvHttpRequest();
    //处理请求
    void handlerHttpRequest();
    //构建响应
    void buildHttpResponse();
    //发送响应
    void sendHttpResponse();
    ~EndPoint()
    {}

private:
    void readRequestLine();
};