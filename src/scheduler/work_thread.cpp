#include "common/block_queue.h"
#include "scheduler/task.h"
#include "scheduler/framework_pool.h"

/// choose a task and assign it to cellet
void* ScheduleProcessor(void* unused) {
    return NULL;
}

void* EventProcesseor(void* unused) {
    return NULL;
}

void* TaskProcessor(void* unused) {
    while(true) {
        TaskPtr task;
        // fetch a task from buffer
        TaskBuffer::Instance()->PopFront(&task);
        // add task into correspond framework
        FrameworkMgr::Instance()->AddTask(task);
    }
    return NULL;
}
