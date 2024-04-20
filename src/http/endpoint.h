#pragma once
#include<array>
#include "http_request.h"
#include "http_response.h"
class EndPoint {
private:
    int _sock;
    HttpRequest _http_request;
    HttpResponse _http_response;
    bool _is_stop;
    std::string _err;
public:
    EndPoint(int sock)
        :_sock(sock),
        _is_stop(false),
        _err("")
    {}
    void read_request();
    void handle_request();
    void build_response();
    bool send_response();
    bool is_stop() const;
    std::string get_error() const;
    std::array<std::string, 2> get_session_msg() const;
    ~EndPoint()
    {}

private:
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