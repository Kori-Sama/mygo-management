#ifndef __LOG_H
#define __LOG_H

#include <iostream>
#include <string>

void inner_log(const char* level, std::string message, const char* file_name, int line) {
    std::cout << "[" << level << "][" << time(nullptr) << "][" << message << "][" << file_name << "][" << line << "]" << std::endl;
}

#define INFO(message) inner_log("INFO", message, __FILE__, __LINE__)
#define WARN(message) inner_log("WARN", message, __FILE__, __LINE__)
#define ERROR(message) inner_log("ERROR", message, __FILE__, __LINE__)
#define FATAL(message) inner_log("FATAL", message, __FILE__, __LINE__)

#endif // __LOG_H