#include <string>
#include <stdexcept>
#include <string.h>
#include "common/cond.h"

using std::string;
using std::runtime_error;

Cond::Cond() {
    CheckError("Cond::Cond", pthread_cond_init(&m_cond, NULL));
}

Cond::~Cond() {
    pthread_cond_destroy(&m_cond);
}

void Cond::Signal() {
    CheckError("Cond::Signal", pthread_cond_signal(&m_cond));
}

void Cond::Wait(Mutex& mutex) {
    CheckError("Cond::Wait", pthread_cond_wait(&m_cond, &(mutex.m_lock)));
}

void Cond::CheckError(const char* info, int code) {
    if (code != 0) {
        string msg = info;
        msg += " error: ";
        msg += strerror(code);
        throw runtime_error(msg);
    }
}

