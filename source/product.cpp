#include "product.h"

namespace inflation {
namespace node {

std::set<std::string> VALID_CURRENCIES = {
    "USD"
};

static const std::string UID_KEY = "uid";
static const std::string TITLE_KEY = "title";
static const std::string DESCRIPTION_KEY = "description";
static const std::string CURRENCY_KEY = "currency";
static const std::string PRICE_KEY = "price";
static const std::string IN_STOCK_KEY = "in_stock";
static const std::string SKU_KEY = "sku";
static const std::string UPC_KEY = "upc";
static const std::string MAKE_KEY = "make";
static const std::string UNIT_KEY = "unit";

void to_json(nlohmann::json& j, const Product& p) {
    j = nlohmann::json{
        {UID_KEY, p.uid},
        {TITLE_KEY, p.title},
        {DESCRIPTION_KEY, p.description},
        {CURRENCY_KEY, p.currency},
        {PRICE_KEY, p.price},
        {IN_STOCK_KEY, p.in_stock}
    };
    if (p.sku.has_value()) {
        j[SKU_KEY] = p.sku.value();
    }
    if (p.upc.has_value()) {
        j[UPC_KEY] = p.upc.value();
    }
    if (p.make.has_value()) {
        j[MAKE_KEY] = p.make.value();
    }
    if (p.unit.has_value()) {
        j[UNIT_KEY] = p.unit.value();
    }
}

void from_json(const nlohmann::json& j, Product& p) {
    j.at(UID_KEY).get_to(p.uid);
    j.at(TITLE_KEY).get_to(p.title);
    j.at(DESCRIPTION_KEY).get_to(p.description);
    j.at(CURRENCY_KEY).get_to(p.currency);
    j.at(PRICE_KEY).get_to(p.price);
    j.at(IN_STOCK_KEY).get_to(p.in_stock);
    j.at(SKU_KEY).get_to(p.sku);
    j.at(UPC_KEY).get_to(p.upc);
    j.at(MAKE_KEY).get_to(p.make);
    j.at(UNIT_KEY).get_to(p.unit);
}

}  // namespace node
}  // namespace inflation
