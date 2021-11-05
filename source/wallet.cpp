#include "wallet.h"

#include <sstream>
#include <iomanip>
#include <iostream>
#include <vector>

#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include "hex.h"
#include "sha256.h"

namespace inflation {
namespace node {

Wallet::Wallet(const std::string &public_key) : _public_key(public_key), _rsa(RSA_new()) {
    BIO *pbkeybio = BIO_new_mem_buf(public_key.data(), public_key.size());
    _rsa = PEM_read_bio_RSAPublicKey(pbkeybio, &_rsa, NULL, NULL);
    BIO_free(pbkeybio);
}

Wallet::Wallet(const std::string &public_key, const std::string &private_key) : Wallet(public_key) {
    BIO *pkeybio = BIO_new_mem_buf(private_key.data(), private_key.size());
    _rsa = PEM_read_bio_RSAPrivateKey(pkeybio, &_rsa, NULL, NULL);
    BIO_free(pkeybio);
}

Wallet::~Wallet() {
    RSA_free(_rsa);
}

bool Wallet::verify(unsigned char *data, size_t data_length, const std::string &signature) {
    const auto key_size = RSA_size(_rsa);
    unsigned char *buffer = (unsigned char *)malloc(key_size);
    const auto signature_bytes = hexToBytes(signature);
    if (RSA_public_decrypt(key_size, (const unsigned char *)signature_bytes.data(), buffer, _rsa, RSA_NO_PADDING) == -1) {
        free(buffer);
        return false;
    }
    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int digest_length = 0;
    const EVP_MD *digest_alg = EVP_sha256();
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if (!EVP_DigestInit_ex(context, digest_alg, NULL)) {
        free(buffer);
        EVP_MD_CTX_free(context);
        return false;
    }
    if (!EVP_DigestUpdate(context, data, data_length)) {
        free(buffer);
        EVP_MD_CTX_free(context);
        return false;
    }
    if (!EVP_DigestFinal_ex(context, digest, &digest_length)) {
        free(buffer);
        EVP_MD_CTX_free(context);
        return false;
    }
    if (RSA_verify_PKCS1_PSS(_rsa, digest, digest_alg, buffer, -1) == -1) {
        free(buffer);
        EVP_MD_CTX_free(context);
        return false;
    }
    return true;
}

std::optional<std::string> Wallet::sign(unsigned char *data, size_t data_length) {
    auto hash_string = sha256(data, data_length);
    if (!hash_string) {
        return std::nullopt;
    }
    auto digest = hexToBytes(hash_string.value());
    unsigned char *signature = (unsigned char *)malloc(sizeof(unsigned char) * RSA_size(_rsa));
    unsigned int signature_size = 0;
    if (!RSA_sign(NID_sha256, (const unsigned char *)digest.data(), digest.size(), signature, &signature_size, _rsa)) {
        free(signature);
        return std::nullopt;
    }
    return hexFromBytes(signature, signature_size);
}

const std::string &Wallet::publicKey() {
    return _public_key;
}

std::shared_ptr<Wallet> generateWallet() {
    // Create a bignum random generator
    BIGNUM *bne = BN_new();
    if (BN_set_word(bne, RSA_F4) == -1) {
        BN_free(bne);
        return nullptr;
    }
    // Create an RSA keypair
    RSA *r = RSA_new();
    const auto generate_key_ret = RSA_generate_key_ex(r, 2048, bne, NULL);
    if (generate_key_ret == 0) {
        std::cout << "Error generating key pair for wallet: " << ERR_get_error() << std::endl;
        BN_free(bne);
        RSA_free(r);
        return nullptr;
    }
    // Create a PEM public key representation
    BIO *bp_public = BIO_new(BIO_s_mem());
    const auto write_public_key_ret = PEM_write_bio_RSAPublicKey(bp_public, r);
    if (write_public_key_ret == 0) {
        std::cout << "Error generating public key for wallet: " << ERR_get_error() << std::endl;
        BN_free(bne);
        RSA_free(r);
        BIO_free_all(bp_public);
        return nullptr;
    }
    // Create a PEM private key representation
    BIO *bp_private = BIO_new(BIO_s_mem());
    if (PEM_write_bio_RSAPrivateKey(bp_private, r, NULL, NULL, 0, NULL, NULL) != 1) {
        BN_free(bne);
        RSA_free(r);
        BIO_free_all(bp_public);
        BIO_free_all(bp_private);
        return nullptr;
    }
    // Write the PEM public key to a string
    size_t pub_len = BIO_pending(bp_public);
    char *pub_key = (char *)malloc(pub_len + 1);
    BIO_read(bp_public, pub_key, pub_len);
    const std::string public_key(pub_key, pub_len);
    // Write the PEM private key to a string
    size_t private_len = BIO_pending(bp_private);
    char *private_key = (char *)malloc(private_len + 1);
    BIO_read(bp_private, private_key, private_len);
    const std::string private_key_str(private_key, private_len);
    // Cleanup
    free(private_key);
    free(pub_key);
    BN_free(bne);
    RSA_free(r);
    BIO_free_all(bp_public);
    BIO_free_all(bp_private);
    return std::make_shared<Wallet>(public_key, private_key_str);
}

}  // namespace node
}  // namespace inflation
