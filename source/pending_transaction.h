#pragma once

#include <string>
#include <unordered_set>

#include <httplib.h>

#include "seen_product.h"

namespace inflation {
namespace node {

class PendingTransaction {
public:
    PendingTransaction(const httplib::Result &response, const std::string &url);
    virtual ~PendingTransaction();

    bool verify() const;
    void sign();
private:
    const std::string _url;
    const std::string _body;
    std::unordered_set<SeenProduct, SeenProductHasher> _products;
};

}  // namespace node
}  // namespace inflation
