#ifndef __STRING_UTILS_H
#define __STRING_UTILS_H

#include <string>
#include <vector>

namespace utils {
    std::vector<std::string> split(const std::string& str, const std::string& delimiter);

    std::string trim(const std::string& str);

    bool starts_with(const std::string& str, const std::string& prefix);
} // namespace utils

#endif // __STRING_UTILS_H