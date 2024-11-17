#include "SharedMemory.hpp"

#include <thread>
#include <atomic>

class SharedMemoryClient_A : public SharedMemoryClient {
public:
    void WorkLoop() {
        working = true;
        std::string command = "ping";    // user command (ex. "ping")
        while (working) {
            if (AwaitLoop() == -1) {
                std::cout << "NO ANS" << std::endl;
                return;
            }
            shm.SetTag(comms[command].first);
            comms[command].second();
            usleep(1000*1000);
        }
    }

    SharedMemoryClient_A(const char *name) : SharedMemoryClient(name) {}
};

void ClientLoop(SharedMemoryClient_A &client) {
    client.WorkLoop();
}

// IT'S A CLIENT
int main([[maybe_unused]]int argc, [[maybe_unused]]char* argv[]) {
    SharedMemoryClient_A client(MEMNAME);
    std::thread clientThread(ClientLoop, std::ref(client));
    int a;
    std::cin >> a;
    client.Stop();
    clientThread.join();

    return 0;
}
