#pragma once

#include <optional>
#include <string>

namespace inflation {
namespace node {

extern std::optional<std::string> sha256(unsigned char *data, size_t data_length);
extern std::optional<std::string> sha256(const std::string &message);

}  // namespace node
}  // namespace inflation

