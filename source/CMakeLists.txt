find_package(LibXml2 REQUIRED)
add_executable(parsingnode
    main.cpp
    local_http_server.cpp
    local_http_server.h
    server.cpp
    server.h
    connection.cpp
    connection.h
    local_request.cpp
    local_request.h
    request_manager.h
    request_manager.cpp
    wallet.h
    wallet.cpp
    hex.h
    hex.cpp
    pending_transaction.h
    pending_transaction.cpp
    parser.h
    product.h
    product.cpp
    html_parser.h
    html_parser.cpp
    sha256.h
    sha256.cpp
    clean_html.h
    clean_html.cpp
    sxg_parser.h
    sxg_parser.cpp
    seen_product.h
    seen_product.cpp
    seen_product_factory.h
    seen_product_factory.cpp)
target_include_directories(parsingnode PUBLIC "${Boost_INCLUDE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/../libraries/fastText/src")
target_link_libraries(parsingnode
    ${Boost_LIBRARIES}
    uriparser
    httplib
    nlohmann_json
    ${OPENSSL_SSL_LIBRARY}
    ${OPENSSL_CRYPTO_LIBRARY}
    base64
    tidy-static
    ${LIBXML2_LIBRARY}
    sxg_static
    fasttext-static
    ip2location)
