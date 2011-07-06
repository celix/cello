#ifndef SRC_SCHEDULER_TASK_QUEUE_H
#define SRC_SCHEDULER_TASK_QUEUE_H

#include <list>

#include "scheduler/task.h"
#include "common/rwlock.h"

using std::list;

class TaskQueue {
public:
    void PushBack(const TaskPtr& task);
    
    void PopFront(TaskPtr* ptr);

    int Size();
private:
    RWLock m_lock;
    list<TaskPtr> m_list; 
};

#endif
