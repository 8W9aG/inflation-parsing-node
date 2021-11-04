#include "request_manager.h"

#include <boost/algorithm/string.hpp>
#include <httplib.h>
#include <uriparser/Uri.h>

#include <chrono>

namespace inflation {
namespace node {

const auto PRIVACY_HEADERS = std::set<std::string>({
    "cookies",
    "authorization"
});

RequestManager::RequestManager() : _thread(RequestManager::run, this), _local_requests(100) {}

RequestManager::~RequestManager() {}

void RequestManager::add_request(const local_request &request) {
    _local_requests.push(request);
}

void RequestManager::join() {
    _thread.join();
}

void RequestManager::run(RequestManager *request_manager) {
    while (!request_manager->_exit) {
        local_request request;
        if (request_manager->_local_requests.pop(request)) {
            UriUriA uri;
            const char * errorPos;
            if (uriParseSingleUriA(&uri, request.url.c_str(), &errorPos) != URI_SUCCESS) {
                std::cout << "Failed to parse " << request.url << std::endl;
            } else {
                httplib::Client cli(std::string(uri.scheme.first, uri.scheme.afterLast) + "://" + std::string(uri.hostText.first, uri.hostText.afterLast));
                UriPathSegmentStructA * head = uri.pathHead;
                std::string path;
                while (head != NULL) {
                    path += "/" + std::string(head->text.first, head->text.afterLast);
                    head = head->next;
                }
                const auto get_path = path + "?" + std::string(uri.query.first, uri.query.afterLast);
                httplib::Headers headers;
                for (const auto &header : request.headers) {
                    if (PRIVACY_HEADERS.count(boost::algorithm::to_lower_copy(header.name)) > 0) {
                        continue;
                    }
                    headers.insert({header.name, header.value});
                }
                auto response = cli.Get(get_path.c_str(), headers);
                uriFreeUriMembersA(&uri);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

}  // namespace node
}  // namespace inflation
