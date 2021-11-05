#pragma once

#include <thread>
#include <memory>

#include <httplib.h>

#include "request_manager.h"

namespace inflation {
namespace node {

class LocalHttpServer {
public:
    LocalHttpServer(int port, std::shared_ptr<RequestManager> request_manager);
    virtual ~LocalHttpServer();
    
    void join();
private:
    std::thread _thread;
    httplib::Server _server;
    
    static void run(LocalHttpServer *local_http_server, int port);
};

}  // namespace node
}  // namespace inflation
