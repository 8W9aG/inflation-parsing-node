#pragma once

#include <openssl/rsa.h>

#include <string>
#include <memory>
#include <optional>

namespace inflation {
namespace node {

class Wallet {
public:
    Wallet(const std::string &public_key);
    Wallet(const std::string &public_key, const std::string &private_key);
    virtual ~Wallet();

    bool verify(unsigned char *data, size_t data_length, const std::string &signature);
    std::optional<std::string> sign(unsigned char *data, size_t data_length);
    const std::string &publicKey();
private:
    const std::string _public_key;

    RSA *_rsa;
};

extern std::shared_ptr<Wallet> generateWallet();

}  // namespace node
}  // namespace inflation
