#include "sha256.h"

#include <openssl/evp.h>

#include "hex.h"

namespace inflation {
namespace node {

std::optional<std::string> sha256(unsigned char *data, size_t data_length) {
    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_length = 0;
    const EVP_MD *digest_alg = EVP_sha256();
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if (!EVP_DigestInit_ex(context, digest_alg, NULL)) {
        EVP_MD_CTX_free(context);
        return std::nullopt;
    }
    if (!EVP_DigestUpdate(context, data, data_length)) {
        EVP_MD_CTX_free(context);
        return std::nullopt;
    }
    if (!EVP_DigestFinal_ex(context, digest, &digest_length)) {
        EVP_MD_CTX_free(context);
        return std::nullopt;
    }
    auto hex_str = hexFromBytes(digest, digest_length);
    EVP_MD_CTX_free(context);
    return hex_str;
}

std::optional<std::string> sha256(const std::string &message) {
    return sha256((unsigned char *)message.data(), message.size());
}

}  // namespace node
}  // namespace inflation
