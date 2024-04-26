#include "protocol.h"
#include "string_utils.h"
#include "utils.h"

#define HEADER_SEP ": "

namespace http {
    HttpResponse HttpResponse::build(int status_code, std::map<std::string, std::string> header, const std::string& body) {
        HttpResponse response;
        response.status_code = status_code;
        response.status_line = HTTP_VERSION;
        response.status_line += " ";
        response.status_line += std::to_string(status_code);
        response.status_line += " ";
        response.status_line += code_to_desc(status_code);
        response.status_line += LINE_END;

        for (auto& kv : header) {
            response.header += kv.first;
            response.header += HEADER_SEP;
            response.header += kv.second;
            response.header += LINE_END;
        }

        response.body = body;

        return response;
    }

    // std::unordered_map<std::string, std::string>& HttpResponse::header_kv() {
    //     static std::unordered_map<std::string, std::string> kv_map;

    //     auto lines = utils::split(header, HEADER_SEP);

    //     for (auto& line : lines) {
    //         auto kv = utils::split(line, LINE_END);
    //         if (kv.size() != 2) {
    //             continue;
    //         }
    //         kv[0] = utils::trim(kv[0]);
    //         kv[1] = utils::trim(kv[1]);

    //         kv_map.insert({ kv[0], kv[1] });
    //     }

    //     return kv_map;
    // }

    HttpRequest HttpRequest::parse(const std::string& request_str) {
        HttpRequest request;

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
            return request;
        }

        request.request_line = lines[0];
        auto request_line = utils::split(request.request_line, " ");
        if (request_line.size() != 3) {
            return request;
        }

        request.method = request_line[0];
        request.uri = request_line[1];
        request.version = request_line[2];

        for (size_t i = 1; i < lines.size(); i++) {
            if (lines[i].empty()) {
                request.blank = lines[i];
                request.body = lines[i + 1];
                break;
            }
            auto kv = utils::split(lines[i], HEADER_SEP);
            if (kv.size() != 2) {
                continue;
            }
            request.header_kv.insert({ kv[0], kv[1] });
        }

        auto uri = utils::split(request.uri, "?");
        request.path = uri[0];
        if (uri.size() > 1) {
            request.query_string = uri[1];
        }

        auto content_length = request.header_kv.find("Content-Length");
        if (content_length != request.header_kv.end()) {
            request.content_length = std::stoi(content_length->second);
        }

        return request;
    }
}