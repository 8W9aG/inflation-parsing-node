#include "seen_product.h"

namespace inflation {
namespace node {

static const std::string PRODUCT_KEY = "product";
static const std::string EMBEDDING_KEY = "embedding";
static const std::string BASKET_KEY = "basket";
static const std::string COUNTRY_KEY = "country";
static const std::string STATE_KEY = "state";

void to_json(nlohmann::json& j, const SeenProduct& p) {
    j = nlohmann::json{
        {PRODUCT_KEY, p.product},
        {EMBEDDING_KEY, p.embedding},
        {BASKET_KEY, p.basket},
        {COUNTRY_KEY, p.country},
        {STATE_KEY, p.state}
    };
}

void from_json(const nlohmann::json& j, SeenProduct& p) {
    j.at(PRODUCT_KEY).get_to(p.product);
    j.at(EMBEDDING_KEY).get_to(p.embedding);
    j.at(BASKET_KEY).get_to(p.basket);
    j.at(COUNTRY_KEY).get_to(p.country);
    j.at(STATE_KEY).get_to(p.state);
}

}  // namespace node
}  // namespace inflation
