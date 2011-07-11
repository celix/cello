#include "collector/machine_pool.h"

void MachinePool::Insert(const MachinePtr& machine) {
    string endpoint = machine->GetEndpoint();
    WriteLocker locker(m_lock);
    m_pool[endpoint] = machine;
}

void MachinePool::MapToDo(MachineFunc func) {
    ReadLocker locker(m_lock);
    for (map<string, Machine>::iterator it = m_pool.begin();
         it != m_pool.end(); ++it)
        func(it->second);
}

void MachinePool::Delete(const string& endpoint) {
    WriteLocker locker(m_lock);
    m_pool.erase(endpoint);
}
