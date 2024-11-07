#include "SharedMemory.hpp"
#include "../definitions.hpp"

#include <map>

using namespace std;

// IT'S A CLIENT
int main(int argc, char* argv[]) {
    std::map <std::string, int> comms;
    comms["ping"]    = TAG_PING;
    comms["exit"]    = TAG_EXIT;
    comms["status"]  = TAG_STATUS;
    comms["setfreq"] = TAG_SETFREQ;
    comms["setpwr"]  = TAG_SETPOWER;
    comms["getfreq"] = TAG_GETFREQ;
    comms["getpwr"]  = TAG_GETPOWER;
    comms["setflt"]  = TAG_SETFAULT;
    comms["clrflt"]  = TAG_CLRFAULT;
    comms["listflt"] = TAG_LISTFAULT;

    SharedMemory shm(MEMNAME);

    std::cout << "print something" << std::endl;
    std::string command;
    bool working = true;
    while (working) {
        std::cin >> command;
        if (shm.GetState() == SM_CLIENT) {
            int32_t val;
            switch (comms[command])
            {
            case TAG_PING:
                shm.SetTag(TAG_PING);
                shm.SetState(SM_SERVER);
                std::cout << shm.RecieveStream() << std::endl;
                break;
            case TAG_EXIT:
                shm.SetTag(TAG_EXIT);
                shm.SetState(SM_SERVER);
                working = false;
                break;
            case TAG_STATUS:
                shm.SetTag(TAG_STATUS);
                shm.SetState(SM_SERVER);
                std::cout << shm.RecieveStream() << std::endl;
                break;
            case TAG_SETFREQ:
                shm.SetTag(TAG_SETFREQ);
                std::cin >> val;
                shm.SetNumber(val);
                shm.SetState(SM_SERVER);
                std::cout << shm.RecieveStream() << std::endl;
                break;
            case TAG_SETPOWER:
                shm.SetTag(TAG_SETPOWER);
                std::cin >> val;
                shm.SetNumber(val);
                shm.SetState(SM_SERVER);
                std::cout << shm.RecieveStream() << std::endl;
                break;
            case TAG_GETFREQ:
                shm.SetTag(TAG_GETFREQ);
                shm.SetState(SM_SERVER);
                std::cout << shm.RecieveStream() << std::endl;
                break;
            case TAG_GETPOWER:
                shm.SetTag(TAG_GETPOWER);
                shm.SetState(SM_SERVER);
                std::cout << shm.RecieveStream() << std::endl;
                break;
            case TAG_SETFAULT:
                shm.SetTag(TAG_SETFAULT);
                std::cin >> val;
                shm.SetNumber(val);
                shm.SetState(SM_SERVER);
                std::cout << shm.RecieveStream() << std::endl;
                break;
            case TAG_CLRFAULT:
                shm.SetTag(TAG_CLRFAULT);
                std::cin >> val;
                shm.SetNumber(val);
                shm.SetState(SM_SERVER);
                std::cout << shm.RecieveStream() << std::endl;
                break;
            case TAG_LISTFAULT:
                shm.SetTag(TAG_LISTFAULT);
                shm.SetState(SM_SERVER);
                std::cout << shm.RecieveStream() << std::endl;
                break;
            default:
                std::cout << "wrong" << std::endl;
                break;
            }
        }
        usleep(333000);
    }

    cout << "exiting" << endl;

    return 0;
}
