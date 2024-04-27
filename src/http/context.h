#ifndef __HTTP_CONTEXT_H
#define __HTTP_CONTEXT_H

#include "protocol.h"

namespace http {
    class Context {
        struct Request {
            std::string method;
            std::string url;
            std::string body;
            std::unordered_map<std::string, std::string> header;
            std::string ip;
            int port;
        };
    public:
        Context(HttpRequest& request, HttpResponse& response) :
            _request(request),
            _response(response),
            _is_block(false)
        {
            req.method = request.method;
            req.url = request.uri;
            req.body = request.body;
            req.header = request.header_kv;
        }

        /**
         * @brief Get the value of the query string
         * @param key The key of the query string
         * @return The value of the query string
        */
        std::string query(const char* key) const {
            return _request.query_kv[key];
        }

        /**
         * @brief Set the status code of the response
         * @param code The status code
         * @return The context itself
        */
        Context& code(HttpCode code) {
            _response.status_code = code;
            return *this;
        }

        /**
         * @brief Set the header of the response
         * @param key The key of the header
         * @param value The value of the header
         * @return The context itself
        */
        Context& header(const char* key, const char* value) {
            _response.set_header(key, value);
            return *this;
        }

        /**
         * @brief Send the response
         * @param body The body of the response
        */
        void send(const char* body) {
            _response.body = body;
        }

        /**
         * @brief Send the response with HTML format
         * @param body The body of the response
        */
        void html(const char* body) {
            _response.body = body;
            _response.header_kv["Content-Type"] = "text/html";
        }

        void redirect(const char* url) {
            _response.status_code = HttpCode::FOUND;
            _response.header_kv["Location"] = url;
        }

        /**
         * @brief Block the context. The context will not pass to the next middleware
        */
        void block() {
            _is_block = true;
        }

        bool is_block() const {
            return _is_block;
        }

        Request req;
    private:
        HttpRequest& _request;
        HttpResponse& _response;
        bool _is_block;
    };

    typedef std::function<void(Context&)> HandleFunc;
}

#endif // __HTTP_CONTEXT_H