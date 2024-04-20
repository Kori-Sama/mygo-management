#pragma once
#include<array>
#include<functional>
#include "http_request.h"
#include "http_response.h"
#include "context.h"

typedef std::function<void(Context&)> HandleFunc;

struct ApiHandler {
    std::string url;
    std::string method;
    HandleFunc handler;
    ApiHandler(std::string method, std::string url, HandleFunc handler)
        :url(url),
        method(method),
        handler(handler)
    {}
};

class EndPoint {
private:
    int _sock;
    HttpRequest _http_request;
    HttpResponse _http_response;
    bool _is_stop;
    std::string _err;
    std::vector<ApiHandler*> _handlers;
public:
    EndPoint(int sock)
        :_sock(sock),
        _is_stop(false),
        _err("")
    {}
    void read_request();
    void handle_request();
    bool is_stop() const;
    std::string get_error() const;
    std::array<std::string, 2> get_session_msg() const;

    void register_handlers(std::vector<ApiHandler*>& handlers) {
        this->_handlers = handlers;
    }

    ~EndPoint()
    {}

private:
    void handle_static_request();
    void build_static_response();
    bool send_static_response();
    void handle_api_request();
    void build_api_response();
    bool send_api_response();

    void parse_request_line();
    void parse_request_header();
    bool read_request_line();
    bool read_request_header();
    bool read_request_body();
    bool have_to_read_request_body();

    // int process_cgi();
    // int process_no_cgi();

    void build_ok_response();
    void handle_error(std::string page);
};