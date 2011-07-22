#include <assert.h>

#include "cellet/executor_pool.h"

bool ExecutorPool::Find(const ExecutorPtr& ptr) {
    ReadLocker locker(m_lock);
    map<int64_t, ExecutorPtr>::iterator it = m_executor_map.find(ptr->GetId());
    return it != m_executor_map.end();
}

void ExecutorPool::Insert(const ExecutorPtr& ptr) {
    WriteLocker locker(m_lock);
    m_executor_map[ptr->GetId()] = ptr;
}

void ExecutorPool::StartExecutor() {
    ReadLocker locker(m_lock);
    for (map<int64_t, ExecutorPtr>::iterator it = m_executor_map.begin();
         it != m_executor_map.end(); ++it) {
        // find a wait executor
        if ((it->second)->GetStatus() == EXECUTOR_WAIT) {
            (it->second)->Start();
            return;
        }
    }
}
