#include "protocol.h"
#include "string_utils.h"
#include "http_utils.h"

#define HEADER_SEP ": "

namespace http {

    void HttpResponse::build() {
        status_line = HTTP_VERSION;
        status_line += " ";
        status_line += std::to_string(status_code);
        status_line += " ";
        status_line += code_to_desc(status_code);
        status_line += LINE_END;

        if (header_kv["Content-Length"].empty()) {
            header_kv["Content-Length"] = std::to_string(body.size());
        }

        if (header_kv["Content-Type"].empty()) {
            header_kv["Content-Type"] = "application/json";
        }

        for (auto& kv : header_kv) {
            this->header += kv.first;
            this->header += HEADER_SEP;
            this->header += kv.second;
            this->header += LINE_END;
        }
    }

    void HttpResponse::set_header(const char* key, const char* value) {
        header_kv.insert({ key,value });
    }

    std::string HttpResponse::to_string() {
        std::string response_str;
        response_str += status_line;
        response_str += header;
        response_str += blank;
        response_str += body;

        return response_str;
    }

    bool HttpRequest::parse(const std::string& request_str) {
        std::vector<std::string> lines;
        size_t pos = 0;
        for (size_t i = 0; i < request_str.size(); i++) {
            if (request_str[i] == '\r' && request_str[i + 1] == '\n') {
                lines.push_back(request_str.substr(pos, i - pos));
                pos = i + 2;
                i++;
            } else if (request_str[i] == '\n') {
                lines.push_back(request_str.substr(pos, i - pos));
                pos = i + 1;
            }

            if (i == request_str.size() - 1) {
                lines.push_back(request_str.substr(pos, i - pos + 1));
            }
        }

        if (lines.size() < 1) {
            return false;
        }

        request_line = lines[0];
        auto line_elements = utils::split(request_line, " ");
        if (line_elements.size() != 3) {
            return false;
        }

        method = line_elements[0];
        uri = line_elements[1];
        version = line_elements[2];

        for (size_t i = 1; i < lines.size(); i++) {
            if (lines[i].empty()) {
                blank = lines[i];
                body = lines[i + 1];
                break;
            }
            auto kv = utils::split(lines[i], HEADER_SEP);
            if (kv.size() != 2) {
                continue;
            }
            header_kv.insert({ kv[0], kv[1] });
        }

        auto paths = utils::split(uri, "?");
        uri = paths[0];
        if (paths.size() > 1) {
            query_string = paths[1];
            auto query_kv_str = utils::split(query_string, "&");
            for (auto& kv_str : query_kv_str) {
                auto kv = utils::split(kv_str, "=");
                if (kv.size() != 2) {
                    continue;
                }
                query_kv.insert({ kv[0], kv[1] });
            }
        }

        auto content_length_kv = header_kv.find("Content-Length");
        if (content_length_kv != header_kv.end()) {
            content_length = std::stoi(content_length_kv->second);
        }

        return true;
    }
}