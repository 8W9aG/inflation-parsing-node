#include <iostream>

#include "local_http_server.h"
#include "server.h"

int main(int argc, char *argv[]) {
    inflation::node::LocalHttpServer local_http_server(61563);
    inflation::node::Server server(30668);
    local_http_server.join();
    server.join();
    return 0;
}
