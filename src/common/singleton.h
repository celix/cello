#ifndef SRC_INCLUDE_SINGLETON_H
#define SRC_INCLUDE_SINGLETON_H

#include "common/mutex.h"

template <typename T>
class Singleton {
public:
    static T* Instance() {
        MutexLocker locker(m_locker);
        if (m_instance == NULL)
            m_instance = new T;
        return m_instance;
    }
private:
    static Mutex m_lock;
    static T* m_instance;
};

template <typename T>
T* Singleton::m_instance = NULL;
Mutex Singleton::m_lock;

#endif
