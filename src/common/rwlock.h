#ifndef SRC_COMMON_RWLOCK_H
#define SRC_COMMON_RWLOCK_H

#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <stdexcept>

using std::string;
using std::runtime_error;

namespace cello {

    class RWLock {
        public:
            RWLock() {
                CheckError("Rwlock::Rwlock", pthread_rwlock_init(&m_lock, NULL));
            }

            ~RWLock() {
                pthread_rwlock_destroy(&m_lock);
            }

            void ReadLock() {
                CheckError("RWLock::ReadLock", pthread_rwlock_rdlock(&m_lock));
            }

            void WriteLock() {
                CheckError("RWLock::WriteLock", pthread_rwlock_wrlock(&m_lock));
            }

            void Unlock() {
                CheckError("RWLock::Unlock", pthread_rwlock_unlock(&m_lock));
            }

        private:
            void CheckError(const char* info, int code) {
                if (code != 0) {
                    string msg = info;
                    msg += " error: ";
                    msg += strerror(code);
                    throw runtime_error(msg);
                }
            }

        private:
            pthread_rwlock_t m_lock;
    };

    class ReadLocker {
        public:
            explicit ReadLocker(RWLock& rwlock) : m_rwlock(&rwlock) {
                m_rwlock->ReadLock();
            }

            ~ReadLocker() {
                m_rwlock->Unlock();
            }

        private:
            RWLock* m_rwlock;
    };

    class WriteLocker {
        public:
            explicit WriteLocker(RWLock& rwlock) : m_rwlock(&rwlock) {
                m_rwlock->WriteLock();
            }

            ~WriteLocker() {
                m_rwlock->Unlock();
            }

        private:
            RWLock* m_rwlock;
    };
}

#endif
