#include "seen_product_factory.h"

namespace inflation {
namespace node {

SeenProductFactory::SeenProductFactory(const std::string &model, const std::string &ip2location_bin) : _ip2location(IP2Location_open((char *)ip2location_bin.c_str())) {
    _fastText.loadModel(model);
}

SeenProductFactory::~SeenProductFactory() {
    IP2Location_close(_ip2location);
}

SeenProduct SeenProductFactory::createSeenProduct(const Product &product, const std::string &ip) {
    std::istringstream str(product.title + " " + product.description);
    fasttext::Vector vec(_fastText.getDimension());
    _fastText.getSentenceVector(str, vec);
    std::vector<float> embeddings(vec.data(), vec.data() + vec.size());
    // TODO: Load FastText model for classifying on basket of goods once trained
    const std::string basket = "car";
    IP2LocationRecord *record = IP2Location_get_all(_ip2location, (char *)ip.c_str());
    SeenProduct seen_product(product, embeddings, basket, std::string(record->country_short), std::string(record->region));
    IP2Location_free_record(record);
    return seen_product;
}

}  // namespace node
}  // namespace inflation
