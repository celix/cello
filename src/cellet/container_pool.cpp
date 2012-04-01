#include "cellet/container_pool.h"
#include <glog/logging.h>

using cello::ReadLocker;
using cello::WriteLocker;

void ContainerPool::Insert(const ContainerPtr& ptr) {
    WriteLocker locker(m_lock);
    m_container_pool[ptr->GetPid()] = ptr;
}

void ContainerPool::Delete(pid_t pid) {
    WriteLocker locker(m_lock);
    m_container_pool.erase(pid);
}

bool ContainerPool::FindToDo(pid_t pid, ContainerFunc func) {
    ReadLocker locker(m_lock);
    map<pid_t, ContainerPtr>::iterator it = m_container_pool.find(pid);
    if (it != m_container_pool.end()) {
        func((it->second).get());
        return true;
    }
    LOG(ERROR) << "cant find container with pid: " << pid;
    return false;
}

void ContainerPool::MapToDo(ContainerFunc func) {
    ReadLocker locker(m_lock);
    for (map<pid_t, ContainerPtr>::iterator it = m_container_pool.begin();
         it != m_container_pool.end(); ++it)
        func((it->second).get());
}

bool ContainerPool::DeleteByTaskId(int64_t task_id) {
    WriteLocker locker(m_lock);
    for (map<pid_t, ContainerPtr>::iterator it = m_container_pool.begin();
            it != m_container_pool.end(); ++it) {
        if (it->second->GetId() == task_id) {
            if (it->second->Recycle() >= 0) {
                m_container_pool.erase(it);
                return true;
            } else {
                return false;
            }
        }
    }
    return false;
}

