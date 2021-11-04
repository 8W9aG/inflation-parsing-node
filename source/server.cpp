#include "server.h"

#include <iostream>

#include <boost/bind.hpp>

namespace inflation {
namespace node {

Server::Server(int port) :
    _thread(Server::run, this, port),
    _acceptor(_io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
    start_accept();
}

Server::~Server() {}

void Server::join() {
    _thread.join();
}

void Server::handle_accept(std::shared_ptr<Connection> connection, const boost::system::error_code& err) {
    if (!err) {
        connection->start();
    }
    start_accept();
}

void Server::start_accept() {
    std::shared_ptr<Connection> connection = std::make_shared<Connection>(_io_service);
    _acceptor.async_accept(connection->socket(),
                           boost::bind(&Server::handle_accept,
                                     this,
                                     connection,
                                     boost::asio::placeholders::error));
}

void Server::run(Server *server, int port) {
    std::cout << "Listening to blockchain traffic on " << port << std::endl;
    server->_io_service.run();
}

}  // namespace node
}  // namespace inflation
