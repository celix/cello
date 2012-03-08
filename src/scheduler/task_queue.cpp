#include "scheduler/task_queue.h"

using cello::ReadLocker;
using cello::WriteLocker;

int TaskQueue::Size() {
    ReadLocker locker(m_lock);
    return m_list.size();
}

void TaskQueue::PushBack(const TaskPtr& task) {
    WriteLocker locker(m_lock);
    m_list.push_back(task);
}

void TaskQueue::PopFront(TaskPtr* ptr) {
    WriteLocker locker(m_lock);
    *ptr = m_list.front();
    m_list.pop_front();
}

bool TaskQueue::Erase(int64_t task_id) {
    WriteLocker locker(m_lock);
    for (list<TaskPtr>::iterator it = m_list.begin(); it != m_list.end(); ++it)
        // find the task
        if (task_id == (*it)->GetId()) {
            m_list.erase(it);
            return true;
        }
    // not found return false
    return false;
}

string TaskQueue::GetTaskAddress(int64_t task_id) {
    ReadLocker locker(m_lock);
    for (list<TaskPtr>::iterator it = m_list.begin(); it != m_list.end(); ++it)
        if (task_id == (*it)->GetId())
            return (*it)->GetAddress();
    return string();
}
