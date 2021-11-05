#pragma once

#include <optional>
#include <set>
#include <string>

namespace inflation {
namespace node {

#define COUNT_PRODUCT_OPTIONALS(product) ((product.sku ? 1 : 0) + (product.upc ? 1 : 0) + (product.make ? 1 : 0) + (product.unit ? 1 : 0))

extern std::set<std::string> VALID_CURRENCIES;

struct Product {
    std::string uid;
    std::string title;
    std::string description;
    std::string currency;
    float price;
    bool in_stock;
    std::optional<std::string> sku;
    std::optional<std::string> upc;
    std::optional<std::string> make;
    std::optional<std::string> unit;
    
    Product(const std::string &uid,
            const std::string &title,
            const std::string &description,
            const std::string &currency,
            float price,
            bool in_stock,
            std::optional<std::string> &sku,
            std::optional<std::string> &upc,
            std::optional<std::string> &make,
            std::optional<std::string> &unit) :
        uid(uid),
        title(title),
        description(description),
        currency(currency),
        price(price),
        in_stock(in_stock),
        sku(sku),
        upc(upc),
        make(make),
        unit(unit) {
            assert(VALID_CURRENCIES.count(currency) != 0);
        }
    
    bool operator==(const Product &other) const {
        return uid == other.uid;
    }
    
    bool operator>(const Product &other) const {
        return COUNT_PRODUCT_OPTIONALS((*this)) > COUNT_PRODUCT_OPTIONALS(other);
    }
    
    bool operator<(const Product &other) const {
        return COUNT_PRODUCT_OPTIONALS((*this)) < COUNT_PRODUCT_OPTIONALS(other);
    }
};

struct ProductHasher {
    size_t operator()(const Product &product) const {
        std::hash<std::string> h;
        return h(product.uid);
    }
};

}  // namespace node
}  // namespace inflation
