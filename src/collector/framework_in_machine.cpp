#include <glog/logging.h>
#include "collector/framework_in_machine.h"

using cello::WriteLocker;
using cello::ReadLocker;

void FrameworkInMachine::Insert(ExecutorInMachine* ptr) {
    WriteLocker locker(m_lock);
    m_map[ptr->GetEndpoint()] = ptr;
}

bool FrameworkInMachine::Find(const string& address) {
    ReadLocker locker(m_lock);
    map<string, ExecutorInMachine*>::iterator it = m_map.find(address);
    return it != m_map.end();
}

void FrameworkInMachine::Delete(const string& address) {
    WriteLocker locker(m_lock);
    m_map.erase(address);
}

int FrameworkInMachine::Size() {
    ReadLocker locker(m_lock);
    return m_map.size();
}

void FrameworkInMachine::Push(const ExecutorStat& stat) {
    ReadLocker locker(m_lock);
    for (map<string, ExecutorInMachine*>::iterator it = m_map.begin();
         it != m_map.end(); ++it) {
        if (stat.task_id == it->second->GetId())
            it->second->AddStatInfo(stat);
    }
}

bool FrameworkInMachine::IsOverLoad(int period, double cpu_usage, double proportion) {
    ReadLocker locker(m_lock);
    for (map<string, ExecutorInMachine*>::iterator it = m_map.begin();
         it != m_map.end(); ++it) {
        if (!it->second->IsOverLoad(period, cpu_usage, proportion))
            return false;
    }
    return true;
}

bool FrameworkInMachine::IsIdle(int period, double value, double proportion, int64_t task_id) {
    WriteLocker locker(m_lock);
    for (map<string, ExecutorInMachine*>::iterator it = m_map.begin();
         it != m_map.end(); ++it) {
        if (task_id == it->second->GetId()) {
            bool ret = it->second->IsIdle(period, value, proportion);
            // if idle delete the executor
            if (ret) {
                delete it->second;
                m_map.erase(it);
            }
            return ret;
        }
    }
    LOG(ERROR) << "can't find executor. id: " << task_id;
    return false;

}
