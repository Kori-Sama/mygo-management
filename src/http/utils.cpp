#include<sys/socket.h>
#include <unordered_map>
#include "utils.h"

int utils::read_line(int sock, std::string& out)
{
    char ch = 'X';
    while (ch != '\n') {
        ssize_t size = recv(sock, &ch, 1, 0);
        if (size > 0) {
            if (ch == '\r') {
                //窥探下一个字符是否为\n
                recv(sock, &ch, 1, MSG_PEEK);

                if (ch == '\n') { //下一个字符是\n
                    //\r\n->\n
                    recv(sock, &ch, 1, 0); //将这个\n读走
                } else { //下一个字符不是\n
                    //\r->\n
                    ch = '\n'; //将ch设置为\n
                }
            }
            //普通字符或\n
            out.push_back(ch);
        } else if (size == 0) { //对方关闭连接
            return 0;
        } else { //读取失败
            return -1;
        }
    }
    return out.size(); //返回读取到的字符个数
}

bool utils::cut_string(std::string& target, std::string& key_out, std::string& val_out, std::string sep)
{
    size_t pos = target.find(sep, 0);
    if (pos != std::string::npos) {
        key_out = target.substr(0, pos);
        val_out = target.substr(pos + sep.size());
        return true;
    }
    return false;
}

std::string utils::code_to_desc(int code)
{
    std::string desc;
    switch (code) {
    case 200:
        desc = "OK";
        break;
    case 400:
        desc = "Bad Request";
        break;
    case 404:
        desc = "Not Found";
        break;
    case 500:
        desc = "Internal Server Error";
        break;
    default:
        break;
    }
    return desc;
}

std::string utils::suffix_to_desc(const std::string& suffix)
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
    return "text/html"; //所给后缀未找到则默认该资源为html文件
}

std::vector<std::string> utils::split(std::string str, std::string delimiter)
{
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        tokens.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    if (int(str.length()) > 0) {
        tokens.push_back(str);
    }
    return tokens;
}
