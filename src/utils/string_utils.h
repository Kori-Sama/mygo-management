#ifndef __STRING_UTILS_H
#define __STRING_UTILS_H

#include <string>
#include <vector>

namespace utils {
    /// @brief Split a string by a delimiter
    /// @param str The string to split 
    /// @param delimiter The delimiter to split the string by
    /// @return A vector of strings that are the result of splitting the string by the delimiter
    std::vector<std::string> split(const std::string& str, const std::string& delimiter);

    /// @brief Trim the whitespace from the beginning and end of a string
    /// @param str The string to trim 
    /// @return The string with the whitespace removed from the beginning and end 
    std::string trim(const std::string& str);

    /// @brief Check if a string starts with a prefix
    /// @param str The string to check 
    /// @param prefix The prefix to check if the string starts with 
    /// @return True if the string starts with the prefix, false otherwise 
    bool starts_with(const std::string& str, const std::string& prefix);
} // namespace utils

#endif // __STRING_UTILS_H