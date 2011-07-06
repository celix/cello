#include "common/block_queue.h"
#include "scheduler/task.h"
#include "scheduler/event.h"
#include "scheduler/framework_pool.h"

/// choose a task and assign it to cellet
void* ScheduleProcessor(void* unused) {
    while (true) {
        // pick a task from the framework pool
        TaskPtr task = FrameworkMgr::Instance()->GetTask();
        // has wait task then begin to assign the task
        if (task.get())
            task->AssignTask();
        usleep(1000*100);
    }
    return NULL;
}

void* EventProcesseor(void* unused) {
    while (true) {
        EventPtr event;
        // fetch a event from event queue and handle with it
        EventQueue::Instance()->PopFront(&event);
        event->Handle();
    }
    return NULL;
}

void* TaskProcessor(void* unused) {
    while (true) {
        TaskPtr task;
        // fetch a task from buffer
        TaskBuffer::Instance()->PopFront(&task);
        // add task into correspond framework
        FrameworkMgr::Instance()->AddTask(task);
    }
    return NULL;
}
