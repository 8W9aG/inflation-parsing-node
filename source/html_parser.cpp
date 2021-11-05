#include "html_parser.h"

#include <functional>
#include <map>
#include <regex>

#include <boost/algorithm/string.hpp>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>

#include "clean_html.h"
#include "sha256.h"

namespace inflation {
namespace node {

htmlDocPtr xmlDoc(const std::string &html) {
    return htmlReadMemory(html.data(), html.length(), NULL, NULL, HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING | HTML_PARSE_NONET);
}

std::vector<std::string> xmlXPath(const std::string &xpath, htmlDocPtr doc) {
    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    xmlChar *xpath_xml = BAD_CAST xpath.c_str();
    xmlXPathObjectPtr result = xmlXPathEvalExpression(xpath_xml, context);
    if (xmlXPathNodeSetIsEmpty(result->nodesetval)) {
        xmlXPathFreeObject(result);
        return {};
    }
    xmlNodeSetPtr nodeset = result->nodesetval;
    std::vector<std::string> final_results;
    for (int i = 0; i < nodeset->nodeNr; i++) {
        xmlChar *text = xmlNodeGetContent(nodeset->nodeTab[i]);
        std::string final_text = std::string((const char *)text);
        final_results.push_back(final_text);
        xmlFree(text);
    }
    xmlXPathFreeObject(result);
    return final_results;
}

std::unordered_set<Product, ProductHasher> parseAmazonProduct1(htmlDocPtr doc, const std::string &url) {
    std::unordered_set<Product, ProductHasher> products;
    auto prices = xmlXPath("//span[@id='price_inside_buybox']/text()", doc);
    auto titles = xmlXPath("//span[@id='productTitle']/text()", doc);
    auto descriptions = xmlXPath("//div[@id='productDescription']/p/text()", doc);
    auto skus = xmlXPath("//input[@id='ASIN']/@value", doc);
    auto currencies = xmlXPath("//div[@class='navFooterLine']/a[contains(@href, 'select-currency')]/span[@class='icp-color-base']/text()", doc);
    auto sku = std::make_optional(skus.front());
    std::optional<std::string> upc = std::nullopt;
    std::optional<std::string> make = std::nullopt;
    std::optional<std::string> unit = std::nullopt;
    const auto uid = sha256(std::string("amazon.com") + skus.front()).value();
    auto string_price = prices.front();
    boost::algorithm::trim(string_price);
    string_price.erase(std::remove_if(string_price.begin(), string_price.end(), [](char c) -> bool {
        return c == '$';
    }), string_price.end());
    auto currency = currencies.front();
    boost::algorithm::trim(currency);
    currency = currency.substr(0, currency.find(" "));
    products.insert(Product(uid,
                            titles.front(),
                            descriptions.front(),
                            currency,
                            std::atof(string_price.c_str()),
                            true,
                            sku,
                            upc,
                            make,
                            unit));
    return products;
}

std::vector<std::pair<std::regex, std::function<std::unordered_set<Product, ProductHasher>(htmlDocPtr, const std::string &)>>> HTML_PARSERS = {
    std::make_pair(std::regex(".*.amazon.com.*"), parseAmazonProduct1)
};

HTMLParser::HTMLParser(const httplib::Result &response, const std::string &url) : _doc(xmlDoc(cleanHTML(response->body))), _url(url) {}

HTMLParser::~HTMLParser() {
    xmlFreeDoc(_doc);
}

std::unordered_set<Product, ProductHasher> HTMLParser::parseProducts() {
    std::unordered_set<Product, ProductHasher> final_products;
    for (const auto html_parser : HTML_PARSERS) {
        if (std::regex_match(_url, html_parser.first)) {
            auto products = html_parser.second(_doc, _url);
            for (auto &product : products) {
                auto final_product = final_products.find(product);
                if (final_product == final_products.end()) {
                    final_products.insert(product);
                } else {
                    if (*final_product < product) {
                        final_products.erase(final_product);
                        final_products.insert(product);
                    }
                }
            }
        }
    }
    return final_products;
}

}  // namespace node
}  // namespace inflation
