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

        if (!path.empty()) {
            handle_static(path);
        }
    }

    bool Endpoint::handle_static(const std::string& path) {
        int type = get_path_info(path);
        if (type == -1) {
            return handle_static(DEFAULT_PAGE);
        } else if (type == 0) {
            return handle_static(path + DEFAULT_PAGE);
        } else if (type == 1) {
            _response.status_code = HttpCode::OK;
            auto suffix = utils::split(path, ".")[1];
            _response.header_kv["Content-Type"] = suffix_to_desc(suffix);
            _response.build();
            std::string response_str = _response.to_string_without_body();
            if (write_all(_sock, response_str) <= 0) {
            }
            if (send_file(_sock, path) <= 0) {

            }
            return true;
        }
        return false;
    }

    void Endpoint::send_response() {
        _response.build();
        std::string response_str = _response.to_string();
        if (write_all(_sock, response_str) <= 0) {
            return;
        }
    }

    void Endpoint::build_error_response(HttpCode code) {
        // process error
        _response.status_code = code;
        _response.header_kv["Content-Type"] = "text/html";
        _response.body = "<html><body><h1>" + std::to_string(code) + " " + code_to_desc(code) + "</h1></body></html>";

        send_response();
    }
}