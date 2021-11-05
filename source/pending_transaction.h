#pragma once

#include <string>
#include <unordered_set>

#include <httplib.h>

#include "product.h"

namespace inflation {
namespace node {

class PendingTransaction {
public:
    PendingTransaction(const httplib::Result &response, const std::string &url);
    virtual ~PendingTransaction();

    bool verify() const;
private:
    const std::string _url;
    const std::string _body;
    std::unordered_set<Product, ProductHasher> _products;
};

}  // namespace node
}  // namespace inflation
