#pragma once

#include "parser.h"

#include <httplib.h>
#include <libxml/HTMLparser.h>

namespace inflation {
namespace node {

class HTMLParser : public Parser {
public:
    HTMLParser(const httplib::Result &response, const std::string &url);
    virtual ~HTMLParser();
    
    // Parser
    virtual std::unordered_set<Product, ProductHasher> parseProducts() override;
private:
    htmlDocPtr _doc;
    const std::string _url;
};

}  // namespace node
}  // namespace inflation
