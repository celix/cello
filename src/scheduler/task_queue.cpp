#include "scheduler/task_queue.h"

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
