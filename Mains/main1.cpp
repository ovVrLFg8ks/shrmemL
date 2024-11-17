#include "SharedMemory.hpp"

#include <thread>
#include <atomic>

void servLoop(SharedMemoryServer &server) {
    server.WorkLoop();  // process client requests
}

// IT'S A SERVER
int main([[maybe_unused]]int argc, [[maybe_unused]]char* argv[]) {
    SharedMemoryServer server(MEMNAME);      // init server
    std::thread serveloop(servLoop, std::ref(server));
    int a;
    std::cin >> a;
    server.Stop();
    serveloop.join();

    return 0;
}
