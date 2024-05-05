#include<sys/socket.h>
#include<sys/stat.h>
#include<sys/sendfile.h>
#include <fcntl.h>
#include <unistd.h>
#include <unordered_map>
#include "http_utils.h"


#define BUF_SIZE 1024

namespace http {
    int read_all(int sock, std::string& out) {
        char buf[BUF_SIZE];
        while (true) {
            ssize_t size = recv(sock, buf, sizeof(buf), 0);
            if (size > 0) {
                out.append(buf, size);
                if (size < BUF_SIZE) {
                    break;
                }
            } else if (size == 0) {
                return 0;
            } else {
                return -1;
            }
        }
        return out.size();
    }

    int write_all(int sock, const std::string& data) {
        return send(sock, data.c_str(), data.size(), 0);
    }

    int send_file(int sock, const std::string& path, long size) {
        int fd = open(path.c_str(), O_RDONLY);

        if (fd < 0) {
            return -1;
        }

        int ret = sendfile(sock, fd, nullptr, size);
        close(fd);
        return ret;
    }

    int get_path_info(const std::string& path, long& size) {
        struct stat st;
        if (stat(path.c_str(), &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                return 0;
            } else {
                size = st.st_size;
                return 1;
            }
        }
        return -1;
    }

    std::string code_to_desc(int code) {
        static std::unordered_map<int, std::string> code_desc_map = {
            {200, "OK"},
            {201, "Created"},
            {204, "No Content"},
            {302, "Found"},
            {400, "Bad Request"},
            {401, "Unauthorized"},
            {403, "Forbidden"},
            {404, "Not Found"},
            {405, "Method Not Allowed"},
            {418, "I'm a teapot"},
            {500, "Internal Server Error"}
        };

        auto it = code_desc_map.find(code);
        if (it != code_desc_map.end()) {
            return it->second;
        } else {
            return "Unkown Code";
        }
    }

    std::string suffix_to_desc(const std::string& suffix)
    {
        static std::unordered_map<std::string, std::string> suffix_to_desc = {
                {".html", "text/html"},
                {".css", "text/css"},
                {".js", "application/x-javascript"},
                {".jpg", "application/x-jpg"},
                {".xml", "text/xml"},
                {".svg"," image/svg+xml"},
                {".json", "application/json"},
                {".txt", "text/plain"},
                {".png", "image/png"},
                {".gif", "image/gif"},
                {".jpeg", "image/jpeg"},
                {".ico", "image/x-icon"}
        };
        auto iter = suffix_to_desc.find(suffix);
        if (iter != suffix_to_desc.end()) {
            return iter->second;
        }
        return "text/html";
    }
}