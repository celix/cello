#include <glog/logging.h>
#include <gflags/gflags.h>
#include "collector/trigger.h"
#include "include/proxy.h"
#include "common/rpc.h"

DECLARE_string(scheduler_endpoint);

void Trigger::Action(FrameworkInMachine* fim) {
    if (Condition(fim))
        Operation(fim);
}

bool CpuTrigger::Condition(FrameworkInMachine* fim) {
    // calculate all the executor cpu usage in past m_period_threshold time
    return fim->IsOverLoad(GetPeriod()*60,
                          static_cast<double>(GetValue())/100,
                          m_proportion);
}

bool CpuTrigger::Operation(FrameworkInMachine* fim) {
    LOG(INFO) << "framework " << fim->GetName() << " is over load."
              << " Add executor for it.";
    //add a executor for this framework
    try {
        Proxy<SchedulerClient> proxy =
            Rpc<SchedulerClient, SchedulerClient>::GetProxy(FLAGS_scheduler_endpoint);
        proxy().AddExecutor(fim->GetName());
    } catch (TException &tx) {
        LOG(ERROR) << "rpc error. add executor for framework failed"
                   << " framework: " << fim->GetName();
        return false;
    }
    LOG(INFO) << "add executor succeed. framework: " << fim->GetName();
    return true;
}

bool MemoryTrigger::Condition(FrameworkInMachine* fim) {
    return false;
}

bool MemoryTrigger::Operation(FrameworkInMachine* fim) {
    return true;
}

bool SlotTrigger::Condition(FrameworkInMachine* fim) {
    return false;
}

bool SlotTrigger::Operation(FrameworkInMachine* fim) {
    return true;
}

bool IdleTrigger::Condition(FrameworkInMachine* fim) {
    return fim->IsIdle(GetPeriod()*60, GetValue(), m_id, 1.00);
#if 0
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
#endif
}

bool IdleTrigger::Operation(FrameworkInMachine* fim) {
    LOG(INFO) << "Idle trigger trigged:";
    //add a executor for this framework
    int ret = -1;
    try {
        Proxy<SchedulerClient> proxy =
            Rpc<SchedulerClient, SchedulerClient>::GetProxy(FLAGS_scheduler_endpoint);
        ret = proxy().DeleteExecutor(m_id);
    } catch (TException &tx) {
        LOG(ERROR) << "rpc error. "
                   << "shut down executor failed: "
                   << "framework " << fim->GetName() << " id " << m_id;
        return false;
    }
    if (ret < 0) {
        LOG(ERROR) << "shut down executor failed: "
                   << "framework " << fim->GetName() << " id " << m_id;
        return false;
    } else {
        LOG(INFO) << "add executor succeed: "
                  << "framework " << fim->GetName() << " id " << m_id;
        return true;
    }
}
