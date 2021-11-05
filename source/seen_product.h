#pragma once

#include <vector>

#include <nlohmann/json.hpp>

#include "product.h"

namespace inflation {
namespace node {

struct SeenProduct {
    Product product;
    std::vector<float> embedding;
    std::string basket;
    std::string country;
    std::string state;
    
    SeenProduct(const Product &product,
                std::vector<float> embedding,
                std::string basket,
                std::string country,
                std::string state)
        : product(product),
          embedding(embedding),
          basket(basket),
          country(country),
          state(state) {}
};

struct SeenProductHasher {
    size_t operator()(const SeenProduct &product) const {
        std::hash<std::string> h;
        return h(product.product.uid);
    }
};

extern void to_json(nlohmann::json& j, const SeenProduct& p);
extern void from_json(const nlohmann::json& j, SeenProduct& p);

}  // namespace node
}  // namespace inflation
