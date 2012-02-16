#include "scheduler/autoscale_framework.h"

AutoScaleFramework::AutoScaleFramework(const FrameworkInfoWrapper& info) :
    Framework(info.GetName(),
    info.GetQuota()),
    m_executor_info(info.GetTaskInfo()) {}

void AutoScaleFramework::Init() {
    // add an executor in the queue
    TaskPtr task(new Task(m_executor_info));
    m_wait_queue.PushBack(task);
}

void AutoScaleFramework::PlugTask(TaskPtr& task, queue_type type) {
    // set executor info for the task
    task->SetInfo(m_executor_info);
    PushTask(task, type);
}
