#ifndef __BASE64_H
#define __BASE64_H

#include <vector>
#include <string>
namespace utils {
    typedef unsigned char Byte;

    std::string base64_encode(Byte const* buf, unsigned int bufLen);
    std::vector<Byte> base64_decode(std::string const&);
}
#endif