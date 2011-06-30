#ifndef SRC_SCHEDULER_TASK_POOL_H
#define SRC_SCHEDULER_TASK_POOL_H

#include <map>

#include "scheduler/task.h"
#include "common/rwlock.h"

using std::map;


class TaskPool {
public:
    void Insert(TaskPtr task);

private:
    RWLock m_lock;
    map<int64_t, TaskPtr> m_task_map ;   
};

typedef Singleton<TaskPool> Pool;

#endif
