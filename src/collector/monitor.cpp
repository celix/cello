#include <vector>
#include "collector/monitor.h"

using std::vector;

void Monitor::Start() {
    Thread::ThreadFunc func = bind(&Monitor::Entry, this);
    m_monitor_thread = new Thread(func);
    // start the monitor thread
    m_monitor_thread->Start();
}

void Monitor::Entry() {
    while (true) {
        MachinePtr machine;
        m_queue.PopFront(&machine);
        ForwardEvent(machine);
    }
}

void Monitor::ForwardEvent(const MachinePtr& machine) {
    for (vector<ExecutorStat>::iterator it = machine->executor_list.begin();
         it != machine->executor_list.end(); ++it) {
        map<string, Framework*>::iterator iter = m_framework_pool.find(it->fr_name);
        // framework exists then forward machine
        if (iter != m_framework_pool.end()) {
            iter->second->AddEvent(machine, *it);
        } else {
            // create a new framework, insert into framework map
            Framework* fr = new Framework(it->fr_name);
            m_framework_pool[it->fr_name] = fr;
            fr->AddEvent(machine, *it);
        }
    }
}

void Monitor::AddEvent(const MachinePtr& machine) {
    m_queue.PushBack(machine);
}
