#ifndef SRC_INCLUDE_SINGLETON_H
#define SRC_INCLUDE_SINGLETON_H

#include "common/mutex.h"

using cello::MutexLocker;

template <typename T>
class Singleton {
public:
    static T* Instance() {
        MutexLocker locker(m_lock);
        if (m_instance == NULL)
            m_instance = new T;
        return m_instance;
    }

private:
    static cello::Mutex m_lock;
    static T* m_instance;
};

template <typename T>
T* Singleton<T>::m_instance = NULL;
template <typename T>
cello::Mutex Singleton<T>::m_lock;

#endif
