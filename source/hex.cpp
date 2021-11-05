#include "hex.h"

#include <iomanip>
#include <sstream>

namespace inflation {
namespace node {

std::string hexFromBytes(unsigned char *bytes, size_t len) {
    std::stringstream ss;
    ss << std::hex;
    for (int i = 0; i < len; ++i) {
        ss << std::setw(2) << std::setfill('0') << (int)bytes[i];
    }
    return ss.str();
}

std::vector<char> hexToBytes(const std::string &hex) {
    std::vector<char> bytes;
    for (int i = 0; i < hex.length(); i += 2) {
        std::string byteString = hex.substr(i, 2);
        char byte = (char) strtol(byteString.c_str(), NULL, 16);
        bytes.push_back(byte);
    }
    return bytes;
}

}  // namespace node
}  // namespace inflation
