#pragma once

#include <string>

#include <fasttext.h>
#include <IP2Location.h>

#include "seen_product.h"

namespace inflation {
namespace node {

class SeenProductFactory {
public:
    SeenProductFactory(const std::string &model, const std::string &ip2location_bin);
    virtual ~SeenProductFactory();
    
    SeenProduct createSeenProduct(const Product &product, const std::string &ip_address);
private:
    fasttext::FastText _fastText;
    IP2Location *_ip2location;
};

}  // namespace node
}  // namespace inflation
