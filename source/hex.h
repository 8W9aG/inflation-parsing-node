#pragma once

#include <string>
#include <vector>

namespace inflation {
namespace node {

extern std::string hexFromBytes(unsigned char *bytes, size_t len);
extern std::vector<char> hexToBytes(const std::string &hex);

}  // namespace node
}  // namespace inflation
