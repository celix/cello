#ifndef SRC_CELLET_EXECUTOR_POOL_H
#define SRC_CELLET_EXECUTOR_POOL_H

#include <map>
#include <tr1/memory>
#include <tr1/functional>

#include "common/rwlock.h"
#include "common/singleton.h"
#include "cellet/executor.h"

using std::map;
using std::tr1::function;
using std::tr1::placeholders::_1;
using std::tr1::placeholders::_2;
using cello::RWLock;

class ExecutorPool {
public:
    
    typedef function<void(Executor*)> ExecutorFunc;

    /// @brief: find a executor in process map
    bool Find(const ExecutorPtr& ptr);
    
    /// @brief: insert a executor in map
    void Insert(const ExecutorPtr& ptr);

    /// @brief: delete a executor
    void Delete(int64_t id);
    
    /// @brief: find a waitting executor, and start it
    void StartExecutor();

    bool DeleteExecutor(int64_t task_id);

    bool FindToDo(int64_t id, ExecutorFunc func);

private:
    RWLock m_lock;    
    map<int64_t, ExecutorPtr> m_executor_map;
};

typedef Singleton<ExecutorPool> ExecutorMgr;

#endif
