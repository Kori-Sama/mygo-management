#include "string_utils.h"

namespace utils {
    std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
        std::vector<std::string> result;

        if (str.empty()) {
            return result;
        }

        std::string::size_type start = 0;
        std::string::size_type end = str.find(delimiter);

        while (end != std::string::npos) {
            result.push_back(str.substr(start, end - start));
            start = end + delimiter.length();
            end = str.find(delimiter, start);
        }

        result.push_back(str.substr(start));
        return result;
    }

    std::string trim(const std::string& str) {
        std::string::size_type start = 0;
        std::string::size_type end = str.size();

        while (start < str.size() && std::isspace(str[start])) {
            start++;
        }

        while (end > 0 && std::isspace(str[end - 1])) {
            end--;
        }

        return str.substr(start, end - start);
    }

    bool starts_with(const std::string& str, const std::string& prefix) {
        if (str.size() < prefix.size()) {
            return false;
        }

        return str.compare(0, prefix.size(), prefix) == 0;
    }

} // namespace utils