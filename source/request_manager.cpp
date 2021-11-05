#include "request_manager.h"

#include <boost/algorithm/string.hpp>
#include <uriparser/Uri.h>
#include <openssl/aes.h>
#include <base64.h>

#include <chrono>

#include "hex.h"
#include "pending_transaction.h"

namespace inflation {
namespace node {

const auto PRIVACY_HEADERS = std::set<std::string>({
    "cookies",
    "authorization"
});

RequestManager::RequestManager(const std::shared_ptr<Wallet> &wallet) :
    _wallet(wallet),
    _thread(RequestManager::run, this),
    _local_requests(100) {}

RequestManager::~RequestManager() {}

void RequestManager::add_request(const local_request &request) {
    _local_requests.push(request);
}

void RequestManager::join() {
    _thread.join();
}

void RequestManager::makeRequest(const local_request &request) {
    // Parse request URL
    UriUriA uri;
    const char * errorPos;
    if (uriParseSingleUriA(&uri, request.url.c_str(), &errorPos) != URI_SUCCESS) {
        std::cout << "Failed to parse " << request.url << std::endl;
    } else {
        // Create HTTP client
        const std::string host = std::string(uri.hostText.first, uri.hostText.afterLast);
        httplib::SSLClient cli(host);
        cli.set_connection_timeout(10);
        // Find URL path
        UriPathSegmentStructA * head = uri.pathHead;
        std::string path;
        while (head != NULL) {
            path += "/" + std::string(head->text.first, head->text.afterLast);
            head = head->next;
        }
        const std::string query = std::string(uri.query.first, uri.query.afterLast);
        if (!query.empty()) {
            path += "?" + query;
        }
        // Filter out HTTP headers
        httplib::Headers headers;
        for (const auto &header : request.headers) {
            if (PRIVACY_HEADERS.count(boost::algorithm::to_lower_copy(header.name)) > 0) {
                continue;
            }
            headers.insert({header.name, header.value});
        }
        // Insert wallet address into header
        std::string value;
        std::string encryption_key;
        embedWalletInRequest(headers, value, encryption_key);
        // Perform HTTP request
        if (auto response = cli.Get(path.c_str(), headers)) {
            if (response->status == 200) {
                std::shared_ptr<PendingTransaction> transaction = std::make_shared<PendingTransaction>(response, request.url);
                // TODO: Send this to the server once verified
            }
        } else {
            std::cout << "Response Error: " << response.error() << std::endl;
        }
        uriFreeUriMembersA(&uri);
    }
}

void RequestManager::embedWalletInRequest(httplib::Headers &headers, std::string &value, std::string &encryption_key) {
    const size_t encryption_key_len = 128;
    encryption_key = "";
    for (int i = 0; i < encryption_key_len / 8; ++i) {
        int character_type = random() % 3;
        if (character_type == 0) {
            encryption_key += 'a' + (random() % ('z' - 'a'));
        } else if (character_type == 1) {
            encryption_key += 'A' + (random() % ('Z' - 'A'));
        } else if (character_type == 2) {
            encryption_key += '0' + (random() % ('9' - '0'));
        }
    }
    AES_KEY key;
    AES_set_encrypt_key((const unsigned char *)encryption_key.c_str(), encryption_key_len, &key);
    const auto public_key = _wallet->publicKey();
    unsigned char *encrypted_buffer = (unsigned char *)malloc(sizeof(unsigned char) * public_key.length());
    AES_encrypt((const unsigned char *)public_key.c_str(), encrypted_buffer, &key);
    value = base64_encode(encrypted_buffer, public_key.length());
    // TODO: Make an array of potential request headers
    headers.insert({"X-Request-Id", value});
    free(encrypted_buffer);
}

void RequestManager::run(RequestManager *request_manager) {
    while (!request_manager->_exit) {
        local_request request;
        if (request_manager->_local_requests.pop(request)) {
            std::cout << "Processing local request: " << request.url << std::endl;
            request_manager->makeRequest(request);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

}  // namespace node
}  // namespace inflation
