#include<sys/socket.h>
#include<sys/stat.h>
#include<sys/sendfile.h>
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
        size_t size = data.size();
        size_t pos = 0;
        while (pos < size) {
            ssize_t ret = send(sock, data.c_str() + pos, size - pos, 0);
            if (ret < 0) {
                return -1;
            }
            pos += ret;
        }
        return 1;
    }

    char* read_file(const std::string& path) {
        FILE* fp = fopen(path.c_str(), "rb");
        if (fp == nullptr) {
            return nullptr;
        }

        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        char* buffer = new char[size + 1];
        fread(buffer, 1, size, fp);
        buffer[size] = '\0';

        fclose(fp);
        return buffer;
    }

    bool is_dir(const std::string& path) {
        struct stat st;
        if (stat(path.c_str(), &st) == 0) {
            return S_ISDIR(st.st_mode);
        }
        return false;
    }

    int send_file(int sock, const std::string& path) {

        FILE* fp = fopen(path.c_str(), "rb");
        if (fp == nullptr) {
            return -1;
        }

        fseek(fp, 0, SEEK_END);

        long size = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        return sendfile(sock, fileno(fp), nullptr, size);
    }

    int get_path_info(const std::string& path) {
        struct stat st;
        if (stat(path.c_str(), &st) == 0) {
            if (S_ISDIR(st.st_mode)) {
                return 0;
            } else {
                return 1;
            }
        }
        return -1;
    }

    std::string code_to_desc(int code) {
        static std::unordered_map<int, std::string> code_desc_map = {
            {200, "OK"},
            {400, "Bad Request"},
            {401, "Unauthorized"},
            {403, "Forbidden"},
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