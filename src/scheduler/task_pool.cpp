#include "scheduler/task_pool.h"

using cello::ReadLocker;
using cello::WriteLocker;

void TaskPool::Insert(const TaskPtr& task) {
    WriteLocker locker(m_lock);
    m_task_map[task->GetId()] = task;
}

void TaskPool::Delete(int64_t task_id) {
    WriteLocker locker(m_lock);
    m_task_map.erase(task_id);
}

bool TaskPool::Find(int64_t task_id, TaskPtr* ptr) {
    ReadLocker locker(m_lock);
    map<int64_t, TaskPtr>::iterator it = m_task_map.find(task_id);
    if (it != m_task_map.end()) {
        *ptr = it->second;
        return true;
    }
    return false;
}

bool TaskPool::FindToDo(int64_t task_id, TaskFunc func) {
    ReadLocker locker(m_lock);
    map<int64_t, TaskPtr>::iterator it = m_task_map.find(task_id);
    if (it != m_task_map.end()) {
        func(it->second);
        return true;
    }
    return false;
}
