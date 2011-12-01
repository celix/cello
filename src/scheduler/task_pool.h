#ifndef SRC_SCHEDULER_TASK_POOL_H
#define SRC_SCHEDULER_TASK_POOL_H

#include <map>
#include <tr1/memory>
#include <tr1/functional>

#include "scheduler/task.h"
#include "common/rwlock.h"

using std::map;
using std::tr1::function;
using std::tr1::placeholders::_1;
using std::tr1::placeholders::_2;
using cello::RWLock;


class TaskPool {
public:
    typedef function<void(const TaskPtr&)> TaskFunc;

    void Insert(const TaskPtr& task);
    
    void Delete(int64_t task_id);
    
    /// @brief: find task according to task id
    bool Find(int64_t task_id, TaskPtr* ptr);

    /// @brief: find task and do sth
    bool FindToDo(int64_t task_id, TaskFunc func);

private:
    RWLock m_lock;
    map<int64_t, TaskPtr> m_task_map ;   
};

typedef Singleton<TaskPool> Pool;

#endif
