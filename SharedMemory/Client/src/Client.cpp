#include "Client.hpp"

// returns -1 if server not answering and 0 otherwise
int Client::AwaitLoop() {       
    for (int i = 0; i < awaitIter; i++) {
        if (shm.GetState() == SM_SERVER)
            usleep(sleepMs*1000);
        else
            return 0;   // server switched memory state
    }
    return -1;  // waited too long
}

void Client::Ping() {
    shm.SetState(SM_SERVER);
    std::cout << shm.RecieveStreamFromServer() << std::endl;
}

void Client::Exit() {
    shm.SetState(SM_SERVER);
    working = false;    // exit WorkLoop()
}

void Client::Status() {
    shm.SetState(SM_SERVER);
    std::cout << shm.RecieveStreamFromServer() << std::endl;
}

void Client::SetFrequency() {
    int32_t val;
    std::cin >> val;
    shm.SetNumber(val);
    shm.SetState(SM_SERVER);
    std::cout << shm.RecieveStreamFromServer() << std::endl;
}

void Client::SetPower() {
    int32_t val;
    std::cin >> val;
    shm.SetNumber(val);
    shm.SetState(SM_SERVER);
    std::cout << shm.RecieveStreamFromServer() << std::endl;
}

void Client::GetFrequency() {
    shm.SetState(SM_SERVER);
    if (AwaitLoop() == -1) {
        std::cout << "server not anwering" << std::endl;
        shm.SetState(SM_CLIENT);
    }
    else {
        std::cout << shm.GetNumber() << std::endl;
    }
}

void Client::GetPower() {
    shm.SetState(SM_SERVER);
    if (AwaitLoop() == -1) {
        std::cout << "server not anwering" << std::endl;
        shm.SetState(SM_CLIENT);
    }
    else {
        std::cout << shm.GetNumber() << std::endl;
    }
}

void Client::SetFault() {
    int32_t val;
    std::cin >> val;
    shm.SetNumber(val);
    shm.SetState(SM_SERVER);
    std::cout << shm.RecieveStreamFromServer() << std::endl;
}

void Client::ClearFault() {
    int32_t val;
    std::cin >> val;
    shm.SetNumber(val);
    shm.SetState(SM_SERVER);
    std::cout << shm.RecieveStreamFromServer() << std::endl;
}

void Client::ListFault() {
    shm.SetState(SM_SERVER);
    std::cout << shm.RecieveStreamFromServer() << std::endl;
}

void Client::PrintCommandKeys() {
    std::cout << "available commands:\n";
    for (auto i : comms) {
        std::cout << "\t" << i.first << std::endl;
    }
}

void Client::SendStr() {
    std::string s;
    std::cin >> s;
    shm.SendStreamToServer(s);
}

void Client::WorkLoop() {
    working = true;
    std::string command;    // user command (ex. "ping")
    while (working) {
        std::cin >> command;
        if (shm.GetState() == SM_CLIENT) {
            if (comms.count(command) > 0) {
                shm.SetTag(comms[command].first);
                comms[command].second();
            }
            else if (command == "help")
                PrintCommandKeys();
            else
                std::cout << "wrong command" << std::endl;
        }
        usleep(sleepMs*1000);
    }
}
