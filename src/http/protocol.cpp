#include "protocol.h"
#include "string_utils.h"
#include "utils.h"

#define HEADER_SEP ": "

namespace http {
    HttpResponse& HttpResponse::build(int status_code, std::map<std::string, std::string> header, const std::string& body) {
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
    std::map<std::string, std::string>& HttpResponse::header_kv() {
        static std::map<std::string, std::string> kv;

        auto lines = utils::split(header, HEADER_SEP);

        for (auto& line : lines) {
            auto kv = utils::split(line, LINE_END);
            kv[0] = utils::trim(kv[0]);
            kv[1] = utils::trim(kv[1]);
        }
        return kv;
    }
}