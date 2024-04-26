#include "log.h"

void inner_log(const char* level, std::string message, const char* file_name, int line) {
    std::cout << "[" << level << "][" << time(nullptr) << "][" << message << "][" << file_name << "][" << line << "]" << std::endl;
}