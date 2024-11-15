#include "Server.hpp"

void Server::Ping() {       // answer client
    std::cout << "ping-pong" << std::endl;
    shm.SendStreamToClient((char*)"pong");
}

void Server::Exit() {       // exit WorkLoop()
    std::cout << "exit!" << std::endl;
    working = false;
}

void Server::Status() {     // give client server/equipment status
    std::cout << "status?" << std::endl;
    shm.SendStreamToClient(status);
}

void Server::SetFrequency() {   // set frequency by client
    if (shm.GetNumber() >= minFrequency) {
        frequency = shm.GetNumber();
        std::cout << "frequency set: " << frequency << std::endl;
        shm.SendStreamToClient((char*)"ok");
    }
    else {
        shm.SendStreamToClient((char*)"not ok");
        std::cout << "frequency not set" << std::endl;
    }
}

void Server::SetPower() {       // set power by client
    if (shm.GetNumber() > 0) {
        power = shm.GetNumber();
        std::cout << "power set: " << power << std::endl;
        shm.SendStreamToClient((char*)"ok");
    }
    else if (shm.GetNumber() == 0) {
        power = shm.GetNumber();
        std::cout << "shut down pwr" << std::endl;
        shm.SendStreamToClient((char*)"ok, power off");
    }
    else {
        std::cout << "power not set" << std::endl;
        shm.SendStreamToClient((char*)"not ok");
    }
}

void Server::GetFrequency() {   // client gets frequency
    std::cout << "frequency?" << std::endl;
    shm.SetNumber(frequency);
    shm.SetState(SM_CLIENT);
}

void Server::GetPower() {       // client gets power
    std::cout << "power?" << std::endl;
    shm.SetNumber(power);
    shm.SetState(SM_CLIENT);
}

void Server::SetFault() {       // client sets fault
    faults.emplace_back(shm.GetNumber(), std::chrono::system_clock::now());
    std::cout << "new fault " << shm.GetNumber() << std::endl;
    shm.SendStreamToClient((char*)"ok");
}

void Server::ClearFault() {     // client clears fault
    int32_t faultCode = shm.GetNumber();
    bool faultFound = false;
    for (size_t i = 0; i < faults.size(); i++) {
        if (faults[i].faultCode == faultCode) {
            faults.erase(faults.begin() + i);
            faultFound = true;
            break;
        }
    }
    if (faultFound) {
        std::cout << "fault " << shm.GetNumber() << " cleared" << std::endl;
        shm.SendStreamToClient((char*)"ok");
    }
    else {
        std::cout << "fault " << shm.GetNumber() << " not found" << std::endl;
        shm.SendStreamToClient((char*)"not found");
    }
}

void Server::ListFaults() {     // send faults table to client 
    std::cout << "faults list?" << std::endl;
    std::string list = "faults:\n";
    if (faults.size() == 0)
        list.append(" empty");
    char buf[20];       // 20 is enough to hold "DD.MM.YYYY HH:MM:SS" and \0
    for (auto fault : faults) {
        std::time_t t = std::chrono::system_clock::to_time_t(fault.time);
        strftime(buf, 20, "%d.%m.%Y %H:%M:%S", localtime(&t));
        list.append(std::to_string(fault.faultCode) + "\t" + buf + '\n');
    }
    shm.SendStreamToClient(list);
}

void Server::RecvStr() {
    shm.SetState(SM_CLIENT);
    std::cout << shm.RecieveStreamFromClient() << std::endl;
}

void Server::WorkLoop() {
    working = true;
    while (working) {
        if (shm.GetState() == SM_SERVER) {
            if (comms.count(shm.GetTag()) > 0)
                comms[shm.GetTag()]();
            else {
                std::cout << "bad tag" << std::endl;
                shm.SetState(SM_CLIENT);
            }
        }
        usleep(sleepMs * 1000);
    }
}
