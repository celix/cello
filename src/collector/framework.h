#ifndef SRC_COLLECTOR_FRAMEWORK_H
#define SRC_COLLECTOR_FRAMEWORK_H

#include <list>
#include <map>
#include <string>

#include "collector/trigger.h"
#include "collector/trigger_queue.h"
#include "collector/framework_in_machine.h"
#include "collector/machine.h"
#include "common/thread.h"
#include "common/block_queue.h"

using std::list;
using std::map;
using std::string;
using cello::Thread;
using cello::BlockQueue;

class Framework {
public:
    explicit Framework(const string& name): m_name(name), m_executor_pool(name) {}  
    
    ~Framework();

    /// @brief: factory method for creating framework
    static Framework* CreateFramework();

    string GetName() const {
        return m_name;
    }

    /// @brief: framework starts working
    void Start();
    
    void AddEvent(const MachinePtr& machine, const ExecutorStat& stat);
    
    // @brief: delete a machine
    void EraseMachine(const string& endpoint);
private:
    /// @brief: framework work entry
    void Entry();

private:
    string m_name;
    Thread* m_thread;                                /// work thread
    FrameworkInMachine m_executor_pool;              /// executor pool
    BlockQueue<ExecutorStat> m_queue;                /// inforamtion queue
    TriggerQueue m_trigger_list;                     /// trigger list
};

#endif
