#include "scheduler/task_pool.h"

void TaskPool::Insert(const TaskPtr& task) {
    WriteLocker locker(m_lock);
    m_task_map[task->GetId()] = task;
}


