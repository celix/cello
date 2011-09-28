#ifndef SRC_COLLECTOR_FRAMEWORK_H
#define SRC_COLLECTOR_FRAMEWORK_H

#include <list>
#include <map>

#include "collector/trigger.h"
#include "collector/machine_pool.h"
#include "common/thread.h"

using std::list;
using std::map;

class Framework {
public:
    /// @brief: framework starts working
    void Start();
    
private:
    /// @brief: framework work entry
    void Entry();

private:
    Thread m_thread;                                 /// work thread
    MachinePool m_executor_pool;                     /// executor pool
    BlockQueue<MachinePtr> m_queue;                  /// inforamtion queue
    list<Trigger> m_trigger_list;                    /// trigger list
};

#endif
