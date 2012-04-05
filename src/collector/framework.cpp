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
    // release the thread resource
    delete m_thread;
}

void Framework::Start() {
    // add Slot Trigger by default
    TriggerPtr ptr(new CpuTrigger);
    m_trigger_list.PushBack(ptr);
    LOG(INFO) << "add a over load trigger for a new framework";
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
            m_executor_pool.Push(stat);
            // trigger begin to work
            m_trigger_list.Map(&m_executor_pool);
            // remove useless trigger
            m_trigger_list.Flush(&m_executor_pool);
        }
    }
}

void Framework::AddEvent(const MachinePtr& machine, const ExecutorStat& stat) {
    // whether machine in framework
    if (!m_executor_pool.Find(machine->GetEndpoint())) {
        ExecutorInMachine* eim = new ExecutorInMachine(stat.fr_name,
                                                       stat.task_id,
                                                       machine->GetEndpoint());
        m_executor_pool.Insert(eim);
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
