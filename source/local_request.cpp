#include "local_request.h"

#include <nlohmann/json.hpp>

namespace inflation {
namespace node {

static const std::string NAME_KEY = "name";
static const std::string VALUE_KEY = "value";
static const std::string URL_KEY = "url";
static const std::string HEADERS_KEY = "headers";

void to_json(nlohmann::json& j, const header& h) {
    j = nlohmann::json{{NAME_KEY, h.name}, {VALUE_KEY, h.value}};
}

void from_json(const nlohmann::json& j, header& h) {
    j.at(NAME_KEY).get_to(h.name);
    j.at(VALUE_KEY).get_to(h.value);
}

void to_json(nlohmann::json& j, const local_request& lr) {
    j = nlohmann::json{{URL_KEY, lr.url}, {HEADERS_KEY, lr.headers}};
}

void from_json(const nlohmann::json& j, local_request& lr) {
    j.at(URL_KEY).get_to(lr.url);
    j.at(HEADERS_KEY).get_to(lr.headers);
}

}  // namespace node
}  // namespace inflation
