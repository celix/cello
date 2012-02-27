#include <vector>
#include <glog/logging.h>
#include "collector/monitor.h"

using std::vector;

void Monitor::Start() {
    LOG(INFO) << "monitor starts to work.";
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
            LOG(INFO) << "add event to framework: " << it->fr_name;
            iter->second->AddEvent(machine, *it);
        } else {
            // create a new framework, insert into framework map
            Framework* fr = new Framework(it->fr_name);
            m_framework_pool[it->fr_name] = fr;
            // start framework
            fr->AddEvent(machine, *it);
            fr->Start();
            LOG(INFO) << "create framework in monitor: " << it->fr_name;
            LOG(INFO) << "framework monitor started: " << it->fr_name;
        }
    }
}

void Monitor::AddEvent(const MachinePtr& machine) {
    m_queue.PushBack(machine);
}
