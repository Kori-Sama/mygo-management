#pragma once
#include <string>
#include <vector>

namespace utils {
    int read_line(int sock, std::string& out);
    bool cut_string(std::string& target, std::string& key_out, std::string& val_out, std::string sep);
    std::string code_to_desc(int code);
    std::string suffix_to_desc(const std::string& suffix);
    std::vector<std::string> split(std::string str, std::string delimiter);
}