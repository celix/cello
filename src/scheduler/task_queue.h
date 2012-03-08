#ifndef SRC_SCHEDULER_TASK_QUEUE_H
#define SRC_SCHEDULER_TASK_QUEUE_H

#include <list>
#include <string>
#include "scheduler/task.h"
#include "common/rwlock.h"

using std::list;
using std::string;
using cello::RWLock;

class TaskQueue {
public:
    void PushBack(const TaskPtr& task);
    
    void PopFront(TaskPtr* ptr);
    
    bool Erase(int64_t task_id);
    
    int Size();

    string GetTaskAddress(int64_t task_id);
private:
    RWLock m_lock;
    list<TaskPtr> m_list; 
};

#endif
