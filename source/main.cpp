#include <iostream>

#include "local_http_server.h"
#include "server.h"
#include "request_manager.h"
#include "wallet.h"

int main(int argc, char *argv[]) {
    const auto wallet = inflation::node::generateWallet();
    std::shared_ptr<inflation::node::RequestManager> request_manager = std::make_shared<inflation::node::RequestManager>(wallet);
    inflation::node::LocalHttpServer local_http_server(61563, request_manager);
    inflation::node::Server server(30668);
    local_http_server.join();
    server.join();
    request_manager->join();
    return 0;
}
