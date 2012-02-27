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

Framework::~Framework() {
    // clear the trigger
    m_trigger_list.Clear();
}

void Framework::Start() {
    // add Slot Trigger by default
    TriggerPtr ptr(new SlotTrigger);
    m_trigger_list.PushBack(ptr);
    LOG(INFO) << "add a slot trigger for a new framework";
    Thread::ThreadFunc func = bind(&Framework::Entry, this);
    m_thread = new Thread(func);
    m_thread->Start();
}


void Framework::Entry() {
    // framework monitor doesn't exit until it is deleted explicitly
    while (true) {
        ExecutorStat stat;
        m_queue.PopFront(&stat);
        if (m_executor_pool.Size() > 0) {
            // trigger begin to work
            TriggerQueue::TriggerFunc func = bind(&Trigger::Action, _1, stat);
            m_trigger_list.MapToDo(func);
        }
    }
}

void Framework::AddEvent(const MachinePtr& machine, const ExecutorStat& stat) {
    // whether machine in framework
    if (!m_executor_pool.Find(machine->GetEndpoint())) {
        m_executor_pool.Insert(machine);
        // create a correspond idle trigger
        TriggerPtr ptr(new IdleTrigger(stat.task_id));
        m_trigger_list.PushBack(ptr);
        LOG(INFO) << "add an executor in monitor with idle trigger. "
                  << " Machine: " << machine->GetEndpoint()
                  << " Framework: " << stat.fr_name
                  << " Id: " << stat.task_id;
    }
    m_queue.PushBack(stat);
}

void Framework::EraseMachine(const string& endpoint) {
    m_executor_pool.Delete(endpoint);
    // delete trigger
    // TODO: check all the trigger about the machine has been removed
}
