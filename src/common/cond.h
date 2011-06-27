#ifndef SRC_COMMON_COND_H
#define SRC_COMMON_COND_H

#include <pthread.h>

#include "common/mutex.h"

class Cond {
public:
    Cond();
    ~Cond();
    
    /// send signal to wake up the thread
    void Signal();

    /// block the thread, waitting to be woken up
    void Wait(Mutex& mutex);

private:
    void CheckError(const char* info, int code);

private:
    pthread_cond_t m_cond;
};

#endif
