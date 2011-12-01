#ifndef SRC_COLLECTOR_MONITOR_H
#define SRC_COLLECTOR_MONITOR_H

#include "common/singleton.h"
#include "common/block_queue.h"
#include <tr1/memory>
#include <tr1/functional>

#include "collector/framework.h"
using std::tr1::function;
using std::tr1::placeholders::_1;
using std::tr1::placeholders::_2;
using std::tr1::bind;

using cello::BlockQueue;

class Monitor {
public:
    Monitor(): m_monitor_thread(NULL) {}
    void Start();
    void AddEvent(const MachinePtr& machine);

private:
    /// @brief: parse machine info and forward it to framework
    void ForwardEvent(const MachinePtr& machine);
    
    /// @brief: monitor thread entry
    void Entry();
private:
    BlockQueue<MachinePtr> m_queue;
    map<string, Framework*> m_framework_pool;
    Thread* m_monitor_thread;
};

typedef Singleton<Monitor> MonitorMgr;

#endif
