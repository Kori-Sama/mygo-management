#include <string>
#include <sstream>
#include <algorithm>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include "endpoint.h"
#include "utils.h"

#define SEP ": "

#define HTTP_VERSION "HTTP/1.0"

#define PAGE_400 "400.html"
#define PAGE_404 "404.html"
#define PAGE_500 "500.html"

#define HOME_PAGE "index.html"

std::string static_root = "www";

void EndPoint::read_request()
{
    if (!read_request_line() && !read_request_header()) {
        parse_request_line();
        parse_request_header();
        read_request_body();
    }
}

void EndPoint::handle_request()
{
    if (_http_request.uri.find("/api") == 0) {
        handle_api_request();
        build_api_response();
        send_api_response();
    } else {
        handle_static_request();
        build_static_response();
        send_static_response();
    }
}

void EndPoint::handle_static_request()
{
    auto& code = _http_response.status_code;

    if (_http_request.method == "GET") {
        size_t pos = _http_request.uri.find('?');
        if (pos != std::string::npos) {
            utils::cut_string(_http_request.uri,
                _http_request.path,
                _http_request.query_string, "?");
        } else {
            _http_request.path = _http_request.uri;
        }
    } else if (_http_request.method == "POST") {
        _http_request.path = _http_request.uri;
    } else {
        code = Code::BAD_REQUEST;
        return;
    }

    std::string path = _http_request.path;
    _http_request.path = static_root;
    _http_request.path += path;

    if (_http_request.path[_http_request.path.size() - 1] == '/') {
        _http_request.path += HOME_PAGE;
    }

    struct stat st;
    if (stat(_http_request.path.c_str(), &st) == 0) {
        if (S_ISDIR(st.st_mode)) {
            _http_request.path += "/";
            _http_request.path += HOME_PAGE;
            stat(_http_request.path.c_str(), &st);
        }
    } else {
        size_t pos = _http_request.path.find_last_of('/');
        auto p = _http_request.path.substr(0, pos + 1) + HOME_PAGE;
        _http_request.path = p;
        if (stat(p.c_str(), &st) != 0) {
            code = Code::NOT_FOUND;
            return;
        }
    }

    _http_response.size = st.st_size;

    size_t pos = _http_request.path.rfind('.');
    if (pos == std::string::npos) {
        _http_response.suffix = ".html";
    } else {
        _http_response.suffix = _http_request.path.substr(pos);
    }

    _http_response.fd = open(_http_request.path.c_str(), O_RDONLY);
    if (_http_response.fd >= 0) {
        code = Code::OK;
        return;
    }
    code = Code::INTERNAL_SERVER_ERROR;
}

void EndPoint::handle_api_request()
{
    auto& code = _http_response.status_code;
    for (auto handler : _handlers) {
        auto pos = _http_request.uri.find('?');
        if (pos != std::string::npos) {
            utils::cut_string(_http_request.uri,
                _http_request.path,
                _http_request.query_string, "?");
        } else {
            _http_request.path = _http_request.uri;
        }
        if (handler->method == _http_request.method && handler->url == _http_request.path) {
            Context ctx(&_http_request, &_http_response);
            handler->handler(ctx);
            return;
        }
    }
}

void EndPoint::build_api_response()
{
    int code = _http_response.status_code;
    auto& status_line = _http_response.status_line;
    status_line += HTTP_VERSION;
    status_line += " ";
    status_line += std::to_string(code);
    status_line += " ";
    status_line += utils::code_to_desc(code);
    status_line += LINE_END;

    _http_response.size = _http_response.response_body.size();

    build_ok_response();
}

bool EndPoint::send_api_response()
{
    if (send(_sock, _http_response.status_line.c_str(), _http_response.status_line.size(), 0) <= 0) {
        _err = "Failed to send http response status line";
        close(_http_response.fd);
        return _is_stop = true;
    }
    for (auto& iter : _http_response.response_header) {
        if (send(_sock, iter.c_str(), iter.size(), 0) <= 0) {
            _err = "Failed to send http response header";
            close(_http_response.fd);
            return  _is_stop = true;
        }
    }
    if (send(_sock, _http_response.blank.c_str(), _http_response.blank.size(), 0) <= 0) {
        _err = "Failed to send http response blank line";
        close(_http_response.fd);
        return _is_stop = true;
    }

    auto& response_body = _http_response.response_body;
    const char* start = response_body.c_str();
    size_t size = 0;
    size_t total = 0;
    while (total < response_body.size() && (size = send(_sock, start + total, response_body.size() - total, 0)) > 0) {
        total += size;
    }

    close(_http_response.fd);
    return _is_stop;
}

void EndPoint::build_static_response()
{
    int code = _http_response.status_code;
    auto& status_line = _http_response.status_line;
    status_line += HTTP_VERSION;
    status_line += " ";
    status_line += std::to_string(code);
    status_line += " ";
    status_line += utils::code_to_desc(code);
    status_line += LINE_END;

    std::string path = static_root;
    path += "/";
    std::string content_type = "Content-Type: ";
    switch (code) {
    case Code::OK:
        content_type += utils::suffix_to_desc(_http_response.suffix);
        content_type += LINE_END;
        _http_response.response_header.push_back(content_type);
        build_ok_response();
        break;
    case Code::NOT_FOUND:
        path += PAGE_404;
        handle_error(path);
        break;
    case Code::BAD_REQUEST:
        path += PAGE_400;
        handle_error(path);
        break;
    case Code::INTERNAL_SERVER_ERROR:
        path += PAGE_500;
        handle_error(path);
        break;
    default:
        break;
    }
}

bool EndPoint::send_static_response()
{
    if (send(_sock, _http_response.status_line.c_str(), _http_response.status_line.size(), 0) <= 0) {
        _err = "Failed to send http response status line";
        close(_http_response.fd);
        return _is_stop = true;
    }
    for (auto& iter : _http_response.response_header) {
        if (send(_sock, iter.c_str(), iter.size(), 0) <= 0) {
            _err = "Failed to send http response header";
            close(_http_response.fd);
            return  _is_stop = true;
        }
    }
    if (send(_sock, _http_response.blank.c_str(), _http_response.blank.size(), 0) <= 0) {
        _err = "Failed to send http response blank line";
        close(_http_response.fd);
        return _is_stop = true;
    }

    if (sendfile(_sock, _http_response.fd, nullptr, _http_response.size) <= 0) {
        _err = "Failed to send http response body";
        _is_stop = true;
    }
    close(_http_response.fd);
    return _is_stop;
}

bool EndPoint::is_stop() const {
    return _is_stop;
}

std::string EndPoint::get_error() const {
    return _err;
}

std::array<std::string, 2> EndPoint::get_session_msg() const
{
    std::string req = "PATH: " + _http_request.path;
    std::string res = "CODE: " + std::to_string(_http_response.status_code) + " " + utils::code_to_desc(_http_response.status_code);
    std::array<std::string, 2> arr{ req,res };
    return arr;
}


void EndPoint::parse_request_line()
{
    auto& line = _http_request.request_line;

    std::stringstream ss(line);
    ss >> _http_request.method >> _http_request.uri >> _http_request.version;

    auto& method = _http_request.method;
    std::transform(method.begin(), method.end(), method.begin(), toupper);
}

void EndPoint::parse_request_header()
{
    std::string key;
    std::string value;
    for (auto& iter : _http_request.request_header) {
        if (utils::cut_string(iter, key, value, SEP)) {
            _http_request.header_kv.insert({ key, value });
        }
    }
}

bool EndPoint::read_request_line()
{
    auto& line = _http_request.request_line;
    if (utils::read_line(_sock, line) > 0) {
        line.resize(line.size() - 1);
    } else {
        _err = "Failed to read http request line";
        _is_stop = true;
    }
    return _is_stop;
}

bool EndPoint::read_request_header()
{
    std::string line;
    while (true)
    {
        line.clear();
        if (utils::read_line(_sock, line) <= 0) {
            _err = "Failed to read http request header";
            _is_stop = true;
            break;
        }
        if (line == "\n") {
            _http_request.blank = line;
            break;
        }
        line.resize(line.size() - 1);
        _http_request.request_header.push_back(line);
    }
    return _is_stop;
}

bool EndPoint::read_request_body()
{
    if (!have_to_read_request_body()) return _is_stop;

    int content_length = _http_request.content_length;
    auto& body = _http_request.request_body;

    char ch = 0;
    while (content_length) {
        ssize_t size = recv(_sock, &ch, 1, 0);
        if (size > 0) {
            body.push_back(ch);
            content_length--;
        } else {
            _err = "Failed to read http request body";
            _is_stop = true;
            break;
        }
    }
    return _is_stop;
}

bool EndPoint::have_to_read_request_body()
{
    auto& method = _http_request.method;
    if (method == "POST") {
        auto& header_kv = _http_request.header_kv;
        auto iter = header_kv.find("Content-Length");
        if (iter != header_kv.end()) {
            _http_request.content_length = atoi(iter->second.c_str());
            return true;
        }
    }
    return false;
}


void EndPoint::build_ok_response()
{
    std::string content_length = "Content-Length: ";
    content_length += std::to_string(_http_response.size);
    content_length += LINE_END;
    _http_response.response_header.push_back(content_length);
}

void EndPoint::handle_error(std::string page)
{
    _http_response.fd = open(page.c_str(), O_RDONLY);
    if (_http_response.fd > 0) {
        struct stat st;
        stat(page.c_str(), &st);

        std::string content_type = "Content-Type: text/html";
        content_type += LINE_END;
        _http_response.response_header.push_back(content_type);

        std::string content_length = "Content-Length: ";
        content_length += std::to_string(st.st_size);
        content_length += LINE_END;
        _http_response.response_header.push_back(content_length);

        _http_response.size = st.st_size;
    }
}

void set_static_root(std::string root)
{
    static_root = root;
}