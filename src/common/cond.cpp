#include <string>
#include <stdexcept>
#include <string.h>
#include "common/cond.h"

using std::string;
using std::runtime_error;

cello::Cond::Cond() {
    CheckError("Cond::Cond", pthread_cond_init(&m_cond, NULL));
}

cello::Cond::~Cond() {
    pthread_cond_destroy(&m_cond);
}

void cello::Cond::Signal() {
    CheckError("Cond::Signal", pthread_cond_signal(&m_cond));
}

void cello::Cond::Wait(Mutex& mutex) {
    CheckError("Cond::Wait", pthread_cond_wait(&m_cond, &(mutex.m_lock)));
}

int cello::Cond::Wait(Mutex& mutex, size_t timeout) {
    struct timespec time;
    time.tv_sec = timeout;
    time.tv_nsec = 0;
    int ret = pthread_cond_timedwait(&m_cond, &(mutex.m_lock), &time);
    CheckError("Cond::TimeWait", ret);
    return ret;
}

void cello::Cond::CheckError(const char* info, int code) {
    if (code != 0) {
        string msg = info;
        msg += " error: ";
        msg += strerror(code);
        throw runtime_error(msg);
    }
}

