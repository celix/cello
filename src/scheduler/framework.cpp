#include "scheduler/framework.h"

void Framework::AddWaitTask(const TaskPtr& task) {
    m_wait_queue.PushBack(task);
}
