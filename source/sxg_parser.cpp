#include "sxg_parser.h"

namespace inflation {
namespace node {

// TODO: Implement SXG parsing and verification, libsxg can only generate

SXGParser::SXGParser(const httplib::Result &response, const std::string &url) {}

SXGParser::~SXGParser() {}

std::unordered_set<Product, ProductHasher> SXGParser::parseProducts() {
    std::unordered_set<Product, ProductHasher> final_products;
    return final_products;
}

}  // namespace node
}  // namespace inflation
