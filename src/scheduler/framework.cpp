#include "scheduler/framework.h"

void Framework::PushTask(const TaskPtr& task queue_type type) {
    if (type == WAIT_QUEUE)
        m_wait_queue.PushBack(task);
    else if (type == RUN_QUEUE)
        m_run_queue.PushBack(task);
}

bool Framework::PopTask(TaskPtr* ptr, queue_type type) {
    if (type == WAIT_QUEUE) {
        if (m_wait_queue.Size() <= 0)
            return false;
        m_wait_queue.PopFront(ptr);
        return true;
    } else {
        if (m_run_queue.Size() <= 0)
            return false;
        m_run_queue.PopFront(ptr);
        return true;
    }
}
