#include "local_http_server.h"

#include <nlohmann/json.hpp>

#include "local_request.h"

namespace inflation {
namespace node {

LocalHttpServer::LocalHttpServer(int port, std::shared_ptr<RequestManager> request_manager) : _thread(LocalHttpServer::run, this, port) {
    _server.Post("/", [request_manager](const httplib::Request &request, httplib::Response &response, const httplib::ContentReader &content_reader) {
        std::cout << "Received request from local HTTP server" << std::endl;
        response.status = 200;
        response.set_header("Access-Control-Allow-Origin", "*");
        std::string body;
        content_reader([&](const char *data, size_t data_length) {
            body.append(data, data_length);
            return true;
        });
        nlohmann::json j = nlohmann::json::parse(body);
        auto p2 = j.get<local_request>();
        request_manager->add_request(p2);
        return true;
    });
    _server.Options("/", [](const httplib::Request &request, httplib::Response &response) {
        response.status = 200;
        response.set_header("Access-Control-Allow-Origin", "*");
        response.set_header("Access-Control-Allow-Headers", "Content-Type");
        return true;
    });
}

LocalHttpServer::~LocalHttpServer() {}

void LocalHttpServer::join() {
    _thread.join();
}

void LocalHttpServer::run(LocalHttpServer *local_http_server, int port) {
    std::cout << "Listening to local http traffic on " << port << std::endl;
    local_http_server->_server.listen("0.0.0.0", port);
}

}  // namespace node
}  // namespace inflation
