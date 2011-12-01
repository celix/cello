#ifndef SRC_COMMON_COND_H
#define SRC_COMMON_COND_H

#include <pthread.h>

#include "common/mutex.h"

namespace cello {

    class Cond {
        public:
            Cond();
            ~Cond();

            /// send signal to wake up the thread
            void Signal();

            /// block the thread, waitting to be woken up
            void Wait(Mutex& mutex);

            /// block the thread with timeout
            int Wait(Mutex& mutex, size_t timeout);

        private:
            void CheckError(const char* info, int code);

        private:
            pthread_cond_t m_cond;
    };
}

#endif
