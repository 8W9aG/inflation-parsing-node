#pragma once

#include "parser.h"

#include <httplib.h>

namespace inflation {
namespace node {

class SXGParser : public Parser {
public:
    SXGParser(const httplib::Result &response, const std::string &url);
    virtual ~SXGParser();
    
    // Parser
    virtual std::unordered_set<Product, ProductHasher> parseProducts() override;
};

}  // namespace node
}  // namespace inflation
