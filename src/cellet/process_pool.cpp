#include <assert.h>

#include "cellet/process_pool.h"

bool ProcessPool::Find(const ProcessPtr& ptr) {
    ReadLocker locker(m_lock);
    map<int64_t, ProcessPtr>::iterator it = m_process_map.find(ptr->GetId());
    return it != m_process_map.end();
}

void ProcessPool::Insert(const ProcessPtr& ptr) {
    WriteLocker locker(m_lock);
    m_process_map[ptr->GetId()] = ptr;
}
