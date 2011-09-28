#ifndef SRC_COLLECTOR_MONITOR_H
#define SRC_COLLECTOR_MONITOR_H

#include "common/singleton.h"
#include "common/block_queue.h"

class Monitor {
public:
    void Entry(void* param);
    void AddMachine(const MachinePtr& machine);
private:
    BlockQueue<MachinePtr> m_queue;
    Thread m_monitor_thread;
};

typedef Singleton<Monitor> MonitorMgr;

#endif
