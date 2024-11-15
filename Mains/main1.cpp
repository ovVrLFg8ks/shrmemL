#include "Server.hpp"

// IT'S A SERVER
int main([[maybe_unused]]int argc, [[maybe_unused]]char* argv[]) {
    Server server;      // init server
    server.WorkLoop();  // process client requests

    return 0;
}
