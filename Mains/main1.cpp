#include "SharedMemory.hpp"

#include <thread>
#include <atomic>

// IT'S A SERVER
int main([[maybe_unused]]int argc, [[maybe_unused]]char* argv[]) {
    SharedMemoryServer server;      // init server
    server.WorkLoop();  // process client requests

    return 0;
}
