#include "collector/framework.h"


#include <tr1/memory>
#include <tr1/functional>

using std::tr1::function;
using std::tr1::placeholders::_1;
using std::tr1::placeholders::_2;
using std::tr1::bind;

Framework* Framework::CreateFramework() {
    return NULL;
}

void Framework::Start() {
    // add Slot Trigger by default
    m_trigger_list.push_back(SLOTTrigger::Instance());
    Thread::ThreadFunc func = bind(&Framework::Entry, this);
    m_thread = new Thread(func);
    m_thread->Start();
}

void Framework::Entry() {
    // framework has executors
    while (m_executor_pool.Size() > 0) {
        ExecutorStat stat;
        m_queue.PopFront(&stat);
        for (list<Trigger*>::iterator it = m_trigger_list.begin();
             it != m_trigger_list.end(); ++it) {
            // check if the triggers are triggered
            if ((*it)->Condition(stat))
                (*it)->Operation();
        }
    }
}

void Framework::AddEvent(const MachinePtr& machine, const ExecutorStat& stat) {
    // whether machine in framework
    if (!m_executor_pool.Find(machine->GetEndpoint()))
        m_executor_pool.Insert(machine);
    m_queue.PushBack(stat);
}

void Framework::EraseMachine(const string& endpoint) {
    m_executor_pool.Delete(endpoint);
}
