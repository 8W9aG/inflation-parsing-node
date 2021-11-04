#include <thread>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

namespace inflation {
namespace node {

struct header {
    std::string name;
    std::string value;
};

struct local_request {
    std::string url;
    std::vector<header> headers;
};

void to_json(nlohmann::json& j, const header& h);
void from_json(const nlohmann::json& j, header& h);
void to_json(nlohmann::json& j, const local_request& lr);
void from_json(const nlohmann::json& j, local_request& lr);

}  // namespace node
}  // namespace inflation
