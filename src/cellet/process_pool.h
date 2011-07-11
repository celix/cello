#ifndef SRC_CELLET_PROCESS_POOL_H
#define SRC_CELLET_PROCESS_POOL_H

#include <map>

#include "common/rwlock.h"
#include "common/singleton.h"
#include "cellet/process.h"

using std::map;

class ProcessPool {
public:
    /// @brief: find a process in process map
    bool Find(const ProcessPtr& ptr);
    
    /// @brief: insert a process in map
    void Insert(const ProcessPtr& ptr);

private:
    RWLock m_lock;    
    map<int64_t, ProcessPtr> m_process_map;
};

typedef Singleton<ProcessPool> ProcessMgr;

#endif
