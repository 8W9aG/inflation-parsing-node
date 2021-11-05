#pragma once

#include <thread>
#include <atomic>

#include <boost/lockfree/spsc_queue.hpp>
#include <httplib.h>

#include "local_request.h"
#include "wallet.h"

namespace inflation {
namespace node {

class RequestManager {
public:
    RequestManager(const std::shared_ptr<Wallet> &wallet);
    virtual ~RequestManager();
    
    void add_request(const local_request &request);
    void join();
private:
    const std::shared_ptr<Wallet> _wallet;
    
    boost::lockfree::spsc_queue<local_request> _local_requests;
    std::atomic_bool _exit;
    std::thread _thread;
    
    static void run(RequestManager *request_manager);
    
    void parseHTML(const std::string &html);
    void parseJSON(const std::string &json);
    void parseSXG(const void *data, size_t data_length);
    void makeRequest(const local_request &request);
    void embedWalletInRequest(httplib::Headers &headers, std::string &value, std::string &encryption_key);
};

}  // namespace node
}  // namespace inflation
