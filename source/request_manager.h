#include <thread>
#include <atomic>

#include <boost/lockfree/spsc_queue.hpp>

#include "local_request.h"

namespace inflation {
namespace node {

class RequestManager {
public:
    RequestManager();
    virtual ~RequestManager();
    
    void add_request(const local_request &request);
    void join();
private:
    boost::lockfree::spsc_queue<local_request> _local_requests;
    std::atomic_bool _exit;
    std::thread _thread;
    
    static void run(RequestManager *request_manager);
};

}  // namespace node
}  // namespace inflation
