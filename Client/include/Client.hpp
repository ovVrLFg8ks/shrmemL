#include "SharedMemory.hpp"

#include <iostream>
#include <map>

class Client {
private:
    SharedMemory shm;

    bool working = true;    // WorkLoop() state
    int sleepMs = 333;      // WorkLoop() sleep in loop

    int awaitIter = 10;     // loops max number of iteratoins (in case of server not answering)

    int AwaitLoop();

    void Ping();            // ping server

    void Exit();            // server and client shutdown

    void Status();          // check server status

    void SetFrequency();    

    void SetPower();

    void GetFrequency();

    void GetPower();

    void SetFault();

    void ClearFault();

    void ListFault();       // recieve and print table of faults

    void SendStr();         // send stream to server

    // unordered_map makes PrintCommandKeys() print in right order
    typedef std::pair<int, std::function<void()>> p;
    std::unordered_map <std::string, std::pair<int, std::function<void()>>> comms {
        {"ping",    p(TAG_PING,     std::bind(&Client::Ping, this))},
        {"exit",    p(TAG_EXIT,     std::bind(&Client::Exit, this))},
        {"status",  p(TAG_STATUS,   std::bind(&Client::Status, this))},
        {"setfreq", p(TAG_SETFREQ,  std::bind(&Client::SetFrequency, this))},
        {"setpwr",  p(TAG_SETPOWER, std::bind(&Client::SetPower, this))},
        {"getfreq", p(TAG_GETFREQ,  std::bind(&Client::GetFrequency, this))},
        {"getpwr",  p(TAG_GETPOWER, std::bind(&Client::GetPower, this))},
        {"setflt",  p(TAG_SETFAULT, std::bind(&Client::SetFault, this))},
        {"clrflt",  p(TAG_CLRFAULT, std::bind(&Client::ClearFault, this))},
        {"listflt", p(TAG_LISTFAULT,std::bind(&Client::ListFault, this))},
        {"toserv",  p(TAG_STRTOSERV,std::bind(&Client::SendStr, this))}
    };

    void PrintCommandKeys();

public:
    Client() : shm(SharedMemory(MEMNAME)) {}

    void WorkLoop();
};
