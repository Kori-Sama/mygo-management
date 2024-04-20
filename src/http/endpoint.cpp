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

#define WEB_ROOT "wwwroot"
#define HOME_PAGE "index.html"

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
    auto& code = _http_response.status_code;

    if (_http_request.method == "GET") {
        size_t pos = _http_request.uri.find('?');
        if (pos != std::string::npos) {
            utils::cut_string(_http_request.uri,
                _http_request.path,
                _http_request.query_string, "?");

            // _http_request.is_cgi = true;
        } else {
            _http_request.path = _http_request.uri;
        }
    } else if (_http_request.method == "POST") {
        _http_request.path = _http_request.uri;
        // _http_request.is_cgi = true;
    } else {
        code = BAD_REQUEST;
        return;
    }

    std::string path = _http_request.path;
    _http_request.path = WEB_ROOT;
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
        } else if (st.st_mode & S_IXUSR || st.st_mode & S_IXGRP || st.st_mode & S_IXOTH) {
            // _http_request.is_cgi = true;
        }
        _http_response.size = st.st_size;
    } else {
        code = NOT_FOUND;
        return;
    }


    size_t pos = _http_request.path.rfind('.');
    if (pos == std::string::npos) {
        _http_response.suffix = ".html";
    } else {
        _http_response.suffix = _http_request.path.substr(pos);
    }

    _http_response.fd = open(_http_request.path.c_str(), O_RDONLY);
    if (_http_response.fd >= 0) {
        code = OK;
        return;
    }
    code = INTERNAL_SERVER_ERROR;
}

void EndPoint::build_response()
{
    int code = _http_response.status_code;
    auto& status_line = _http_response.status_line;
    status_line += HTTP_VERSION;
    status_line += " ";
    status_line += std::to_string(code);
    status_line += " ";
    status_line += utils::code_to_desc(code);
    status_line += LINE_END;

    std::string path = WEB_ROOT;
    path += "/";
    switch (code) {
    case OK:
        build_ok_response();
        break;
    case NOT_FOUND:
        path += PAGE_404;
        handle_error(path);
        break;
    case BAD_REQUEST:
        path += PAGE_400;
        handle_error(path);
        break;
    case INTERNAL_SERVER_ERROR:
        path += PAGE_500;
        handle_error(path);
        break;
    default:
        break;
    }
}

bool EndPoint::send_response()
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

// int EndPoint::process_cgi()
// {
//     int code = OK; //要返回的状态码，默认设置为200

//     auto& bin = _http_request.path;      //需要执行的CGI程序
//     auto& method = _http_request.method; //请求方法

//     //需要传递给CGI程序的参数
//     auto& query_string = _http_request.query_string; //GET
//     auto& request_body = _http_request.request_body; //POST

//     int content_length = _http_request.content_length;  //请求正文的长度
//     auto& response_body = _http_response.response_body; //CGI程序的处理结果放到响应正文当中

//     //1、创建两个匿名管道（管道命名站在父进程角度）
//     //创建从子进程到父进程的通信信道
//     int input[2];
//     if (pipe(input) < 0) { //管道创建失败，则返回对应的状态码
//         code = INTERNAL_SERVER_ERROR;
//         return code;
//     }
//     //创建从父进程到子进程的通信信道
//     int output[2];
//     if (pipe(output) < 0) { //管道创建失败，则返回对应的状态码
//         code = INTERNAL_SERVER_ERROR;
//         return code;
//     }

//     //2、创建子进程
//     pid_t pid = fork();
//     if (pid == 0) { //child
//         //子进程关闭两个管道对应的读写端
//         close(input[0]);
//         close(output[1]);

//         //将请求方法通过环境变量传参
//         std::string method_env = "METHOD=";
//         method_env += method;
//         putenv((char*)method_env.c_str());

//         if (method == "GET") {
//             std::string query_env = "QUERY_STRING=";
//             query_env += query_string;
//             putenv((char*)query_env.c_str());
//         } else if (method == "POST") {
//             std::string content_length_env = "CONTENT_LENGTH=";
//             content_length_env += std::to_string(content_length);
//             putenv((char*)content_length_env.c_str());
//         } else {
//         }


//         dup2(output[0], 0);
//         dup2(input[1], 1);

//         //4、将子进程替换为对应的CGI程序
//         execl(bin.c_str(), bin.c_str(), nullptr);
//         exit(1); //替换失败
//     } else if (pid < 0) { //创建子进程失败，则返回对应的错误码
//         code = INTERNAL_SERVER_ERROR;
//         return code;
//     } else { //father
//         //父进程关闭两个管道对应的读写端
//         close(input[1]);
//         close(output[0]);

//         if (method == "POST") { //将正文中的参数通过管道传递给CGI程序
//             const char* start = request_body.c_str();
//             int total = 0;
//             int size = 0;
//             while (total < content_length && (size = write(output[1], start + total, request_body.size() - total)) > 0) {
//                 total += size;
//             }
//         }

//         //读取CGI程序的处理结果
//         char ch = 0;
//         while (read(input[0], &ch, 1) > 0) {
//             response_body.push_back(ch);
//         } //不会一直读，当另一端关闭后会继续执行下面的代码

//         //等待子进程（CGI程序）退出
//         int status = 0;
//         pid_t ret = waitpid(pid, &status, 0);
//         if (ret == pid) {
//             if (WIFEXITED(status)) { //正常退出
//                 if (WEXITSTATUS(status) == 0) { //结果正确
//                     code = OK;
//                 } else {
//                     code = BAD_REQUEST;
//                 }
//             } else {
//                 code = INTERNAL_SERVER_ERROR;
//             }
//         }

//         //关闭两个管道对应的文件描述符
//         close(input[0]);
//         close(output[1]);
//     }
//     return code; //返回状态码
// }

// int EndPoint::process_no_cgi()
// {
//     _http_response.fd = open(_http_request.path.c_str(), O_RDONLY);
//     if (_http_response.fd >= 0) {
//         return OK;
//     }
//     return INTERNAL_SERVER_ERROR;
// }

void EndPoint::build_ok_response()
{
    std::string content_type = "Content-Type: ";
    content_type += utils::suffix_to_desc(_http_response.suffix);
    content_type += LINE_END;
    _http_response.response_header.push_back(content_type);

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
