#include <glog/logging.h>
#include <gflags/gflags.h>
#include "collector/trigger.h"
#include "include/proxy.h"
#include "common/rpc.h"

DECLARE_string(scheduler_endpoint);

void Trigger::Action(const ExecutorStat& stat) {
    if (Condition(stat))
        Operation(stat);
}

bool CpuTrigger::Condition(const ExecutorStat& stat) {
    return false;
}

bool CpuTrigger::Operation(const ExecutorStat& stat) {
    return true;
}

bool MemoryTrigger::Condition(const ExecutorStat& stat) {
    return false;
}

bool MemoryTrigger::Operation(const ExecutorStat& stat) {
    return true;
}

bool SlotTrigger::Condition(const ExecutorStat& stat) {
    // first in
    if (stat.task_num <= GetValue() && !m_is_ignited) {
        m_start_time = time(0);
        m_is_ignited = true;
        return false;
    }
    // bomb! trigger is triggered
    if (stat.task_num <= GetValue() && m_is_ignited &&
        time(0) - m_start_time >= GetPeriod() * 60) {
        LOG(WARNING) << "Slot Trigger is triggered, begin to shut down the executor: "
                     << "framework " << stat.fr_name << " id " << stat.task_id;
        // reset the flag
        m_is_ignited = false;
        return true;
    }
    if (stat.task_num > GetValue() && m_is_ignited)
        m_is_ignited = false;
    return false;
}

bool SlotTrigger::Operation(const ExecutorStat& stat) {
    LOG(INFO) << "Slot trigger is trigged.";
    //add a executor for this framework
    try {
        Proxy<SchedulerClient> proxy =
            Rpc<SchedulerClient, SchedulerClient>::GetProxy(FLAGS_scheduler_endpoint);
        proxy().AddExecutor(stat.fr_name);
    } catch (TException &tx) {
        LOG(ERROR) << "rpc error. add executor for framework failed"
                   << " framework: " << stat.fr_name;
        return false;
    }
    LOG(INFO) << "add executor succeed. framework: " << stat.fr_name;
    return true;
}

bool IdleTrigger::Condition(const ExecutorStat& stat) {
    // executor stat is to this trigger
    if (m_id == stat.task_id) {
        // let task num be the quota
        // first in
        if (stat.task_num <= GetValue() && !m_is_ignited) {
            m_start_time = time(0);
            m_is_ignited = true;
            LOG(WARNING) << "Idle Trigger starts to work";
            return false;
        }
        // bomb! trigger is triggered
        if (stat.task_num <= GetValue() && m_is_ignited &&
                (time(0) - m_start_time) >= GetPeriod() * 60) {
            LOG(WARNING) << "Idle Trigger is triggered, begin to shut down the executor: "
                << "framework " << stat.fr_name << " id " << stat.task_id;
            // reset the flag
            m_is_ignited = false;
            return true;
        }
        if (stat.task_num > GetValue() && m_is_ignited)
            m_is_ignited = false;
        return false;
    }
    return false;
}

bool IdleTrigger::Operation(const ExecutorStat& stat) {
    LOG(INFO) << "Idle trigger trigged:";
    //add a executor for this framework
    int ret = -1;
    try {
        Proxy<SchedulerClient> proxy =
            Rpc<SchedulerClient, SchedulerClient>::GetProxy(FLAGS_scheduler_endpoint);
        ret = proxy().DeleteExecutor(stat.task_id);
    } catch (TException &tx) {
        LOG(ERROR) << "rpc error. "
                   << "shut down executor failed: "
                   << "framework " << stat.fr_name << " id " << stat.task_id;
        return false;
    }
    if (ret < 0) {
        LOG(ERROR) << "shut down executor failed: "
                   << "framework " << stat.fr_name << " id " << stat.task_id;
        return false;
    } else {
        LOG(INFO) << "add executor succeed: "
                  << "framework " << stat.fr_name << " id " << stat.task_id;
        return true;
    }
}
