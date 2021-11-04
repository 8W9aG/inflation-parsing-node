#include "connection.h"

#include <iostream>

#include <boost/bind.hpp>

namespace inflation {
namespace node {

Connection::Connection(boost::asio::io_service &io_service) : _socket(io_service) {}

Connection::~Connection() {}

boost::asio::ip::tcp::socket &Connection::socket() {
    return _socket;
}

void Connection::start() {
    _socket.async_read_some(boost::asio::buffer(_data,  sizeof(_data)), boost::bind(&Connection::handle_read, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    _socket.async_write_some(boost::asio::buffer("Hello from the server", sizeof(_data)), boost::bind(&Connection::handle_write, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Connection::handle_read(const boost::system::error_code& err, size_t bytes_transferred) {
    if (!err) {
        std::cout << _data << std::endl;
    } else {
        std::cerr << "error: " << err.message() << std::endl;
        _socket.close();
    }
}

void Connection::handle_write(const boost::system::error_code& err, size_t bytes_transferred) {
    if (!err) {
        std::cout << "Server sent Hello message!"<< std::endl;
    } else {
        std::cerr << "error: " << err.message() << std::endl;
        _socket.close();
    }
}

}  // namespace node
}  // namespace inflation
