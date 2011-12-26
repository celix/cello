#include <assert.h>

#include "scheduler/framework.h"

Framework::Framework(const FrameworkInfoWrapper& info) {
    m_framework_name = info.GetName();
    m_quota = info.GetQuota();
    m_executor_info = info.GetTaskInfo();
}

void Framework::Init() {
    // add an executor in the queue
    TaskPtr task(new Task(m_executor_info));
    m_wait_queue.PushBack(task);
}

void Framework::PushTask(const TaskPtr& task, queue_type type) {
    assert(type == WAIT_QUEUE || type == RUN_QUEUE);
    if (type == WAIT_QUEUE)
        m_wait_queue.PushBack(task);
    else
        m_run_queue.PushBack(task);
}

bool Framework::PopTask(TaskPtr* ptr, queue_type type) {
    assert(type == WAIT_QUEUE || type == RUN_QUEUE);
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

bool Framework::RemoveTask(int64_t task_id, queue_type type) {
    assert(type == WAIT_QUEUE || type == RUN_QUEUE);
    if (type == WAIT_QUEUE)
        return m_wait_queue.Erase(task_id);
    else
        return m_run_queue.Erase(task_id);
}
