#include "SharedMemory.hpp"

#include <iostream>
#include <ctime>
#include <chrono>
#include <map>

class Server {
private:
    std::string status = "OK!";     // server/equipment status
    int32_t frequency = 1000;       // equipment freq
    int32_t minFrequency = 1000;    // 1000 for example 
    int32_t power = 0;              // equipment power

    bool working = true;            // WorkLoop() state
    int sleepMs = 333;              // WorkLoop() sleep in loop

    struct Fault {
        int faultCode;
        //char severity;
        std::chrono::system_clock::time_point time;

        Fault(int faultCode, std::chrono::system_clock::time_point time) : faultCode(faultCode), time(time) {};
    };
    
    std::vector<Fault> faults;
    //std::multiset<Fault> s;
    SharedMemory shm;
    
    void Ping();            // answer client

    void Exit();            // exit WorkLoop()

    void Status();          // give client server/equipment status

    void SetFrequency();    // set frequency by client

    void SetPower();        // set power by client

    void GetFrequency();    // client gets frequency

    void GetPower();        // client gets power

    void SetFault();        // client sets fault

    void ClearFault();      // client clears fault

    void ListFaults();      // send faults table to client

    void RecvStr();

    // map of previous functions to execute in WorkLoop()
    std::map<int, std::function<void()>> comms {
        {TAG_PING,      std::bind(&Server::Ping, this)},
        {TAG_EXIT,      std::bind(&Server::Exit, this)},
        {TAG_STATUS,    std::bind(&Server::Status, this)},
        {TAG_SETFREQ,   std::bind(&Server::SetFrequency, this)},
        {TAG_SETPOWER,  std::bind(&Server::SetPower, this)},
        {TAG_GETFREQ,   std::bind(&Server::GetFrequency, this)},
        {TAG_GETPOWER,  std::bind(&Server::GetPower, this)},
        {TAG_SETFAULT,  std::bind(&Server::SetFault, this)},
        {TAG_CLRFAULT,  std::bind(&Server::ClearFault, this)},
        {TAG_LISTFAULT, std::bind(&Server::ListFaults, this)},
        {TAG_STRTOSERV, std::bind(&Server::RecvStr, this)}
    };

public:
    Server() : shm(SharedMemory(MEMNAME)) {
        shm.SetState(SM_CLIENT);            // pass control to client
        faults.emplace_back(0, std::chrono::system_clock::now());
    }

    /* you can add constructor for your MEMNAME here maybe */

    // processes client requests
    void WorkLoop();
};
