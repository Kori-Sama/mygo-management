#ifndef __HTTP_UTILS_H
#define __HTTP_UTILS_H

#include <string>
#include <vector>

namespace http {
    int read_all(int sock, std::string& out);
    int write_all(int sock, const std::string& data);
    char* read_file(const std::string& path);
    bool is_dir(const std::string& path);

    int send_file(int sock, const std::string& path);

    /**
     * @brief Get the path info
     * @param path The path to get info
     * @return 0 if the path is a directory, 1 if the path is a file, -1 if the path does not exist
    */
    int get_path_info(const std::string& path);

    std::string code_to_desc(int code);
    std::string suffix_to_desc(const std::string& suffix);
}

#endif