#include <glog/logging.h>
#include <gflags/gflags.h>
#include "collector/trigger.h"
#include "include/proxy.h"
#include "common/rpc.h"

DECLARE_string(scheduler_endpoint);

bool CpuTrigger::Condition(const ExecutorStat& stat) {
    return false;
}

bool CpuTrigger::Operation(const string& name) {
    return true;
}

bool MemoryTrigger::Condition(const ExecutorStat& stat) {
    return false;
}

bool MemoryTrigger::Operation(const string& name) {
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

bool SlotTrigger::Operation(const string& name) {
    //add a executor for this framework
    try {
        Proxy<SchedulerClient> proxy =
            Rpc<SchedulerClient, SchedulerClient>::GetProxy(FLAGS_scheduler_endpoint);
        proxy().AddExecutor(name);
    } catch (TException &tx) {
        LOG(ERROR) << "add executor for framework " << name << " failed";
    }
    LOG(INFO) << "add executor succeed for " << name;
    return true;
}

