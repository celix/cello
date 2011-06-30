#include "scheduler/task_queue.h"

int TaskQueue::Size() {
    ReadLocker locker(m_lock);
    return m_list.size();
}

void TaskQueue::PushBack(const TaskPtr& task) {
    WriteLocker locker(m_lock);
    m_list.push_back(task);
}
