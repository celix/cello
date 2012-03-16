#include <assert.h>

#include "scheduler/framework.h"
#include "scheduler/event.h"
#include "scheduler/dispatcher.h"

Framework::Framework(const FrameworkInfoWrapper& info) :
    m_framework_name(info.GetName()), m_quota(info.GetQuota()) {}

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
    if (type == WAIT_QUEUE) {
        return m_wait_queue.Erase(task_id);
    } else {
        return m_run_queue.Erase(task_id);
    }
}

bool Framework::KillTask(int64_t task_id, queue_type type) {
    assert(type == WAIT_QUEUE || type == RUN_QUEUE);
    if (type == WAIT_QUEUE) {
        return m_wait_queue.Erase(task_id);
    } else {
        string address = m_run_queue.GetTaskAddress(task_id);
        // need to send kill task command to cellet
        // kill the process running in cellet
        EventPtr event(new KillActionEvent(address, task_id));
        EventDispatcher::Instance()->Dispatch(event->Type())->PushBack(event);
        return m_run_queue.Erase(task_id);
    }
}
