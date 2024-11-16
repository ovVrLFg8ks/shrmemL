#include "SharedMemory.hpp"

// IT'S A CLIENT
int main([[maybe_unused]]int argc, [[maybe_unused]]char* argv[]) {
    SharedMemoryClient client;
    std::cout << "print something" << std::endl;
    client.WorkLoop();
    std::cout << "exiting" << std::endl;

    return 0;
}
