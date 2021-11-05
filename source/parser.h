#pragma once

#include <unordered_set>

#include "product.h"

namespace inflation {
namespace node {

class Parser {
public:
    virtual std::unordered_set<Product, ProductHasher> parseProducts() = 0;
};

}  // namespace node
}  // namespace inflation
