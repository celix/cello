#ifndef SRC_CELLET_PROCESS_POOL_H
#define SRC_CELLET_PROCESS_POOL_H

#include <map>

#include "include/type.h"
#include "cellet/process.h"

using std::map;

class ProcessPool {
public:
    /// @brief: find a process in specific map
    bool Find(const ProcessPtr& ptr, map_type type);
    
    void Insert(const ProcessPtr& ptr, map_type type);

private:
    map<int64_t, Process> m_wait_map;
    map<int64_t, Process> m_run_map;
}

#endif
