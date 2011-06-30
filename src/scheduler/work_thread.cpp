#include "common/block_queue.h"
#include "scheduler/task.h"

/// choose a task and assign it to cellet
void* ScheduleProcessor(void* unused) {

}

void* EventProcesseor(void* unused) {

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
