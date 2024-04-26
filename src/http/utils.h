#ifndef __HTTP_UTILS_H
#define __HTTP_UTILS_H

#include <string>
#include <vector>

namespace http {
    int read_line(int sock, std::string& out);
    std::string code_to_desc(int code);
    std::string suffix_to_desc(const std::string& suffix);
}

#endif