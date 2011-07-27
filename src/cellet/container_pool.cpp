#include "cellet/container_pool.h"

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
    return false;
}

void ContainerPool::MapToDo(ContainerFunc func) {
    ReadLocker locker(m_lock);
    for (map<pid_t, ContainerPtr>::iterator it = m_container_pool.begin();
         it != m_container_pool.end(); ++it)
        func(it->get());
}
