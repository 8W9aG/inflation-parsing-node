#pragma once

#include <thread>
#include <memory>

#include <boost/asio.hpp>

namespace inflation {
namespace node {

class Connection : public std::enable_shared_from_this<Connection> {
public:
    Connection(boost::asio::io_service &io_service);
    virtual ~Connection();
    
    boost::asio::ip::tcp::socket &socket();
    void start();
private:
    boost::asio::ip::tcp::socket _socket;
    unsigned char _data[131072];
    
    void handle_read(const boost::system::error_code& err, size_t bytes_transferred);
    void handle_write(const boost::system::error_code& err, size_t bytes_transferred);
};

}  // namespace node
}  // namespace inflation
