#include <thread>

#include <httplib.h>

namespace inflation {
namespace node {

class LocalHttpServer {
public:
    LocalHttpServer(int port);
    virtual ~LocalHttpServer();
    
    void join();
private:
    std::thread _thread;
    httplib::Server _server;
    
    static void run(LocalHttpServer *local_http_server, int port);
};

}  // namespace node
}  // namespace inflation
