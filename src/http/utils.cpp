#include<sys/socket.h>
#include <unordered_map>
#include "utils.h"

namespace http {
    int read_line(int sock, std::string& out)
    {
        char ch = 'X';
        while (ch != '\n') {
            ssize_t size = recv(sock, &ch, 1, 0);
            if (size > 0) {
                if (ch == '\r') {
                    recv(sock, &ch, 1, MSG_PEEK);

                    if (ch == '\n') {
                        recv(sock, &ch, 1, 0);
                    } else {
                        ch = '\n';
                    }
                }
                out.push_back(ch);
            } else if (size == 0) {
                return 0;
            } else {
                return -1;
            }
        }
        return out.size();
    }

    std::string code_to_desc(int code) {
        static std::unordered_map<int, std::string> code_desc_map = {
            {200, "OK"},
            {400, "Bad Request"},
            {404, "Not Found"},
            {500, "Internal Server Error"}
        };

        auto it = code_desc_map.find(code);
        if (it != code_desc_map.end()) {
            return it->second;
        } else {
            return "";
        }
    }

    std::string suffix_to_desc(const std::string& suffix)
    {
        static std::unordered_map<std::string, std::string> suffix_to_desc = {
                {".html", "text/html"},
                {".css", "text/css"},
                {".js", "application/x-javascript"},
                {".jpg", "application/x-jpg"},
                {".xml", "text/xml"}
        };
        auto iter = suffix_to_desc.find(suffix);
        if (iter != suffix_to_desc.end()) {
            return iter->second;
        }
        return "text/html";
    }
}