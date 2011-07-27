#ifndef SRC_CELLET_EXECUTOR_POOL_H
#define SRC_CELLET_EXECUTOR_POOL_H

#include <map>

#include "common/rwlock.h"
#include "common/singleton.h"
#include "cellet/executor.h"

using std::map;

class ExecutorPool {
public:
    
    typedef ExectorFunc;
    /// @brief: find a executor in process map
    bool Find(const ExecutorPtr& ptr);
    
    /// @brief: insert a executor in map
    void Insert(const ExecutorPtr& ptr);
    
    /// @brief: find a waitting executor, and start it
    void StartExecutor();

    bool FindToDo(int64_t id, ExecutorFunc func);

private:
    RWLock m_lock;    
    map<int64_t, ExecutorPtr> m_executor_map;
};

typedef Singleton<ExecutorPool> ExecutorMgr;

#endif
