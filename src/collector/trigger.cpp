#include <glog/logging.h>
#include "collector/trigger.h"
#include "include/proxy.h"

bool CpuTrigger::Condition(const ExecutorStat& stat) {
    return false;
}

bool CpuTrigger::Operation() {
    return true;
}

bool MemoryTrigger::Condition(const ExecutorStat& stat) {
    return false;
}

bool MemoryTrigger::Operation() {
    return true;
}

bool SlotTrigger::Condition(const ExecutorStat& stat) {
    // first in
    if (stat.task_num >= m_value_threshold && !m_is_ignited) {
        m_start_time = time(0);
        m_is_ignited = true;
        return false;
    }
    // bomb! trigger is triggered
    if (stat.task_num >= m_value_threshold && m_is_ignited &&
        time(0) - m_start_time >= m_period_threshold * 60) {
        LOG(WARNING) << "Slot Trigger is triggered";
        // reset the flag
        m_is_ignited = false;
        return true;
    }
    if (stat.task_num < m_value_threshold && m_is_ignited)
        m_is_ignited = false;
    return false;
}

bool SlotTrigger::Operation() {
    //add a executor for this framework
  //  Proxy<> proxy = Rpc::GetProxy(FLAGS_scheduler_endpoint, TIME_OUT);
   // proxy()->SubmitTask
    return true;
}


