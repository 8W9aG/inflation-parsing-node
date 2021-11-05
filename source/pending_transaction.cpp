#include "pending_transaction.h"

#include <map>

#include "html_parser.h"
#include "parser.h"

namespace inflation {
namespace node {

std::shared_ptr<Parser> createHTMLParser(const httplib::Result &response, const std::string &url) {
    return std::make_shared<HTMLParser>(response, url);
}

std::map<std::string, std::function<std::shared_ptr<Parser>(const httplib::Result &, const std::string &)>> PARSERS = {
    {"text/html", createHTMLParser}
};

PendingTransaction::PendingTransaction(const httplib::Result &response, const std::string &url) : _url(url), _body(response->body) {
    const auto content_type = response->get_header_value("content-type");
    for (const auto &parser_factory : PARSERS) {
        if (content_type.find(parser_factory.first) != std::string::npos) {
            auto parser = parser_factory.second(response, url);
            _products = parser->parseProducts();
        }
    }
}

PendingTransaction::~PendingTransaction() {}

bool PendingTransaction::verify() const {
    return _products.size() > 0;
}

}  // namespace node
}  // namespace inflation
