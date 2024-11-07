#include "SharedMemory.hpp"

#define SM_WRITE 0
#define SM_READ 1

int SharedMemory::Init() {
    bool exist = false;

    // create shared memory object
    fd = shm_open(memname, O_CREAT | O_EXCL | O_RDWR, S_IREAD | S_IWRITE | S_IEXEC);
    if (errno == EEXIST)
    {
        fd = shm_open(memname, O_RDWR, S_IREAD | S_IWRITE | S_IEXEC);
        exist = true;
    }
    else
    {
        if (ftruncate(fd, sizeof(shrData)) == -1)
            return -1;
    }
    if (fd == -1)
        return -1;

    // new mapping in the VAS
    shmp = (shrData *)mmap(NULL, sizeof(shrData), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shmp == MAP_FAILED)
        return -1;

    if (!exist)
        shmp->state = SM_WRITE;
    return 0;
}

int SharedMemory::Close() {
    if (munmap(shmp, sizeof(shrData)) == -1)
        return -1;

    shm_unlink(memname);

    if (close(fd) == -1)
        return -1;
    return 0;
}

void SharedMemory::int32ToChar(char a[], int32_t n) {
    memcpy(a, &n, 4);
}

int32_t SharedMemory::charToInt32(char a[]) {
    int32_t n = 0;
    memcpy(&n, a, 4);
    return n;
}

void SharedMemory::SendStream(std::string &str) {
    size_t dataLength = str.length();
    for (size_t s = 0; s < dataLength; s += bufsize)
    {
        memcpy(&shmp->data, &str.data()[s], std::min(bufsize, dataLength - s));
        shmp->dataSize = dataLength - s;
        shmp->state = SM_READ;
        while (shmp->state != SM_WRITE)
        {
            usleep(sleepDurationMs * 1000);
        }
    }
    shmp->state = SM_READ;
}

void SharedMemory::SendStream(char *ar) {
    size_t dataLength = strlen(ar);
    for (size_t s = 0; s < dataLength; s += bufsize)
    {
        memcpy(&shmp->data, &ar[s], std::min(bufsize, dataLength - s));
        shmp->dataSize = dataLength - s;
        shmp->state = SM_READ;
        while (shmp->state != SM_WRITE)
        {
            usleep(sleepDurationMs * 1000);
        }
    }
    shmp->state = SM_READ;
}

std::string SharedMemory::RecieveStream() {
    std::string str = "";
    while (true)
    {
        while (shmp->state != SM_READ)
        {
            usleep(sleepDurationMs * 1000);
        }
        str.append(shmp->data, std::min(bufsize, shmp->dataSize));
        if (shmp->dataSize <= bufsize)
            break;
        shmp->state = SM_WRITE;
    }
    shmp->state = SM_WRITE;
    return str;
}

#undef SM_WRITE
#undef SM_READ