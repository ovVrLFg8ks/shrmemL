#include "SharedMemory.hpp"
#include "../definitions.hpp"
#include <ctime>
#include <chrono>

using namespace std;

struct Fault {
    int number;
    std::chrono::system_clock::time_point time;

    Fault(int number, std::chrono::system_clock::time_point time) : number(number), time(time) {};
};

// IT'S A SERVER ! ! !
int main(int argc, char* argv[]) {
    std::string status = "OK!";
    int32_t frequency = 0;
    int32_t power = 0;

    std::vector<Fault> faults;
    faults.emplace_back(0, std::chrono::system_clock::now());
    
    SharedMemory shm(MEMNAME);
    shm.SetState(SM_CLIENT);

    bool working = true;
    while (working) {
        if (shm.GetState() == SM_SERVER) {
            switch (shm.GetTag())
            {
            case TAG_PING:
                std::cout << "ping-pong" << std::endl;
                shm.SendStream("pong");
                break;
            case TAG_EXIT:
                working = false;
                break;
            case TAG_STATUS:
                std::cout << "status?" << std::endl;
                shm.SendStream(status);
                break;
            case TAG_SETFREQ:
                if (shm.GetNumber() >= 1000) {
                    frequency = shm.GetNumber();
                    std::cout << "frequency set: " << frequency << std::endl;
                    shm.SendStream("ok");
                }
                else {
                    shm.SendStream("not ok");
                    std::cout << "frequency not set" << std::endl;
                }
                break;
            case TAG_SETPOWER:
                if (shm.GetNumber() > 0) {
                    power = shm.GetNumber();
                    std::cout << "power set: " << power << std::endl;
                    shm.SendStream("ok");
                }
                else if (shm.GetNumber() == 0) {
                    power = shm.GetNumber();
                    std::cout << "shut down pwr" << std::endl;
                    shm.SendStream("ok, power off");
                }
                else {
                    std::cout << "power not set" << std::endl;
                    shm.SendStream("not ok");
                }
                break;
            case TAG_GETFREQ: {
                std::cout << "frequency?" << std::endl;
                std::string str = std::to_string(frequency);
                shm.SendStream(str);
                break;
            }
            case TAG_GETPOWER: {
                std::cout << "power?" << std::endl;
                std::string str = std::to_string(power);
                shm.SendStream(str);
                break;
            }
            case TAG_SETFAULT:
                faults.emplace_back(shm.GetNumber(), std::chrono::system_clock::now());
                std::cout << "new fault " << shm.GetNumber() << std::endl;
                shm.SendStream("ok");
                break;
            case TAG_CLRFAULT: {
                int32_t t = shm.GetNumber();
                bool found = false;
                for (int i = 0; i < faults.size(); i++) {
                    if (faults[i].number == t) {
                        faults.erase(faults.begin() + i);
                        found = true;
                        break;
                    }
                }
                if (found) {
                    std::cout << "fault " << shm.GetNumber() << " cleared" << std::endl;
                    shm.SendStream("ok");
                }
                else {
                    std::cout << "fault " << shm.GetNumber() << " not found" << std::endl;
                    shm.SendStream("not found");
                }
                break;
            }
            case TAG_LISTFAULT: {
                std::cout << "faults list?" << std::endl;
                std::string list = "faults:\n";
                char buf[20];
                for (auto i : faults) {
                    std::time_t t = std::chrono::system_clock::to_time_t(i.time);
                    strftime(buf, 20, "%d.%m.%Y %H:%M:%S", localtime(&t));
                    list.append(std::to_string(i.number) + "\t" + buf + '\n');
                }
                shm.SendStream(list);
                break;
            }
            default:
                std::cout << "what?" << std::endl;
                shm.SetState(SM_CLIENT);
                break;
            }
        }
        usleep(333000);
    }
    //shm.SendStream(ar);
    

    return 0;
}
