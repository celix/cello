#include "common/block_queue.h"
#include "scheduler/task.h"
#include "scheduler/framework_pool.h"
#include "scheduler/matcher.h"
#include "scheduler/starter.h"

/// choose a task and assign it to cellet
void* ScheduleProcessor(void* unused) {
    while (true) {
        // pick a task from the framework pool
        TaskPtr task = FrameworkMgr::Instance()->GetTask();
        string cellet_address;
        // if match task success then start task
        if (Matcher::MatchTask(task, &cellet_address)) {
            // task start success
            if (Starter::StartTask(cellet_address, task));
        }
        FrameworkMgr::Instance()->AddTask(task);
        
    }
    return NULL;
}

void* EventProcesseor(void* unused) {
    while (true) {
    
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
