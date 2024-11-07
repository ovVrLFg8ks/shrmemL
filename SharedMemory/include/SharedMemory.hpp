#pragma once

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>          //  O_... definitions
#include <string.h>
#include <string>
#include <errno.h>
#include <cstring>
#include <algorithm>

class SharedMemory {
private:
    #define SM_WRITE 0
    #define  SM_READ 1

    const char *memname;
    constexpr static size_t bufsize = 16;   // minimum 4 for numbers
    const int waitTime = 100;
    u_int16_t sleepDurationMs = 100;
    int fd;

    //
    struct shrData {
        char state = SM_WRITE;  
        int tag = 0;
        size_t dataSize = 0;    // for text only
        char data[bufsize] {0};
    };

    shrData *shmp;

    int Init();

    int Close();

    void int32ToChar(char a[], int32_t n);

    int32_t charToInt32(char a[]);

public:

    SharedMemory(const char *memname) : memname(memname) {
        Init();
    };
    
    ~SharedMemory() {
        Close();
    }

    char GetState() { return shmp->state; }

    void SetState(char st) { shmp->state = st; }

    int GetTag() { return shmp->tag; }

    void SetTag(int tag) { shmp->tag = tag; }

    int32_t GetNumber() { return charToInt32(shmp->data); }

    void SetNumber(int32_t num) { int32ToChar(shmp->data, num); }

    void SendStream(std::string &str);

    void SendStream(char *ar);

    std::string RecieveStream();

    #undef SM_WRITE
    #undef SM_READ
};
