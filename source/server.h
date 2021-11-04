#include <thread>

#include <boost/asio.hpp>

#include "connection.h"

namespace inflation {
namespace node {

class Server {
public:
    Server(int port);
    virtual ~Server();
    
    void join();
    void handle_accept(std::shared_ptr<Connection> connection, const boost::system::error_code& err);
private:
    std::thread _thread;
    boost::asio::io_service _io_service;
    boost::asio::ip::tcp::acceptor _acceptor;
    
    void start_accept();
    
    static void run(Server *server, int port);
};

}  // namespace node
}  // namespace inflation
