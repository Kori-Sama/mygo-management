#include "endpoint.h"
#include "http_utils.h"

namespace http {
    void Endpoint::handle() {
        std::string req;
        if (read_all(_sock, req) <= 1) {
            return;
        }

        if (_request.parse(req) < 0) {
            return;
        }

        auto handleFunc = Router::instance().match(_request.uri, _request.method);

        if (handleFunc == nullptr) {
            build_error_response(HttpCode::NOT_FOUND);
            return;
        }

        Context ctx(_request, _response);
        handleFunc(ctx);



        if (_response.status_code == HttpCode::OK) {
            send_response();
        } else {
            build_error_response(_response.status_code);
        }
    }

    void Endpoint::send_response() {
        _response.build();
        std::string response_str = _response.to_string();
        if (write_all(_sock, response_str)) {
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