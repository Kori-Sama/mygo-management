#include "endpoint.h"
#include "http_utils.h"
#include "string_utils.h"


namespace http {
    void Endpoint::handle() {
        std::string req;
        if (read_all(_sock, req) <= 1) {
            return;
        }

        if (_request.parse(req) < 0) {
            return;
        }

        Context ctx(_request, _response);
        ctx.req.ip = _ip;
        ctx.req.port = _port;
        Router::instance().pass_middleware(ctx);
        if (ctx.is_block()) {
            send_response();
            return;
        }

        if (utils::starts_with(_request.uri, "/api")) {
            auto handleFunc = Router::instance().match(_request.uri, _request.method);
            if (handleFunc == nullptr) {
                build_error_response(HttpCode::NOT_FOUND);
                return;
            }
            handleFunc(ctx);

            if (_response.status_code == HttpCode::OK) {
                send_response();
            } else {
                build_error_response(_response.status_code);
            }
            return;
        }

        auto path = Router::instance().route_static(ctx);

        if (path.empty()) {
            return;
        }
        if (ctx.req.method != "GET") {
            build_error_response(HttpCode::METHOD_NOT_ALLOWED);
            return;
        }
        if (!handle_static(path)) {
            build_error_response(HttpCode::NOT_FOUND);
        }
    }

    bool Endpoint::handle_static(std::string path) {
        long size = 0;
        int type = get_path_info(path, size);
        if (type == -1) {
            auto pos = path.rfind("/");
            if (path.substr(pos) == DEFAULT_PAGE) {
                return false;
            }
            path = path.substr(0, pos) + DEFAULT_PAGE;
            return handle_static(path);
        } else if (type == 0) {
            return handle_static(path + DEFAULT_PAGE);
        }

        _response.status_code = HttpCode::OK;
        auto pos = path.rfind(".");
        auto suffix = path.substr(pos);
        _response.header_kv["Content-Type"] = suffix_to_desc(suffix);
        _response.header_kv["Content-Length"] = std::to_string(size);
        _response.build();
        std::string response_str = _response.to_string_without_body();
        if (write_all(_sock, response_str) <= 0) {
        }
        if (send_file(_sock, path, size) <= 0) {

        }
        return true;
    }

    void Endpoint::send_response() {
        _response.build();
        std::string response_str = _response.to_string();
        if (write_all(_sock, response_str) <= 0) {
            return;
        }
    }

    void Endpoint::build_error_response(HttpCode code) {

        _response.status_code = code;
        _response.header_kv["Content-Type"] = "text/html";
        _response.body = "<html><body><h1>" + std::to_string(code) + " " + code_to_desc(code) + "</h1></body></html>";

        send_response();
    }
}