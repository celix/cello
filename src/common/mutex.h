#ifndef SRC_COMMON_MUTEX_H
#define SRC_COMMON_MUTEX_H

#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <stdexcept>

using std::string;
using std::runtime_error;

namespace cello {

    class Mutex {
        public:
            Mutex() {
                CheckError("Mutex::Mutex", pthread_mutex_init(&m_lock, NULL));
            }

            ~Mutex() {
                pthread_mutex_destroy(&m_lock);
            }

            void Lock() {
                CheckError("Mutex::Lock", pthread_mutex_lock(&m_lock));
            }

            void Unlock() {
                CheckError("Mutex::Unlock", pthread_mutex_unlock(&m_lock));
            }

        private:
            void CheckError(const char* info, int err_code) {
                if (err_code != 0) {
                    string msg = info;
                    msg += " error: ";
                    msg += strerror(err_code);
                    throw runtime_error(msg);
                }
            }

        private:
            pthread_mutex_t m_lock;
            friend class Cond;
    };


    class MutexLocker {
        public:
            explicit MutexLocker(Mutex& mutex) : m_mutex(&mutex) {
                m_mutex->Lock();
            }

            ~MutexLocker() {
                m_mutex->Unlock();
            }

        private:
            cello::Mutex* m_mutex;
    };
}

#endif
