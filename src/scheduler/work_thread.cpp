#include "common/block_queue.h"
#include "scheduler/task.h"
#include "scheduler/event.h"
#include "scheduler/framework_pool.h"
#include "scheduler/components_manager.h"

/// choose a task and assign it to cellet
void* ScheduleProcessor(void* unused) {
    while (true) {
        // pick a task from the framework pool
        TaskPtr task = ComponentsMgr::Instance()->GetPool()->GetTask();
        // has wait task then begin to assign the task
        if (task.get()) {
            // task asssign success change task state to started
            if (task->AssignTask())
                task->TaskAssigned();
            else
                ComponentsMgr::Instance()->GetPool()->AddTask(task);
        }
        usleep(1000*50);
    }
    return NULL;
}

#if 0
void* EventProcesseor(void* unused) {
    while (true) {
        EventPtr event;
        // fetch a event from event queue and handle with it
        EventQueue::Instance()->PopFront(&event);
        event->Handle();
    }
    return NULL;
}
#endif

void* TaskProcessor(void* unused) {
    while (true) {
        TaskPtr task;
        // fetch a task from buffer
        TaskBuffer::Instance()->PopFront(&task);
        // add task into correspond framework
        ComponentsMgr::Instance()->GetPool()->PlugTask(task);
    }
    return NULL;
}

