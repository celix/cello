#ifndef SRC_SCHEDULER_IDENTITY_H
#define SRC_SCHEDULER_IDENTITY_H

#include "common/mutex.h"
#include "common/singleton.h"

using cello::Mutex;

class Identity {
public:
    explicit Identity(int num) : m_id(num) {}
    
    Identity() : m_id(0) {}
    
    /// create task id for every task
    int64_t GetTaskId() {
        MutexLocker locker(m_lock);
        return ++m_id;
    }

private:
    Mutex m_lock;
    int64_t m_id;
};

typedef Singleton<Identity> TaskIdentity;

#endif
