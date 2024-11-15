#include "Client.hpp"

// IT'S A CLIENT
int main([[maybe_unused]]int argc, [[maybe_unused]]char* argv[]) {
    Client client;
    std::cout << "print something" << std::endl;
    client.WorkLoop();
    std::cout << "exiting" << std::endl;

    return 0;
}
