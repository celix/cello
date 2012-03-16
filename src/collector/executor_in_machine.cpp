#include <gflags/gflags.h>
#include <glog/logging.h>
#include "collector/executor_in_machine.h"

using cello::WriteLocker;
using cello::ReadLocker;

DECLARE_int32(heartbeat_interval);

void ExecutorInMachine::AddStatInfo(const ExecutorStat& stat) {
    WriteLocker locker(m_lock);
    m_resource_list.push_back(stat);
}

bool ExecutorInMachine::IsOverLoad(int period, double cpu_usage, double proportion) {
    unsigned int len = period/FLAGS_heartbeat_interval;
    ReadLocker locker(m_lock);
    // executor run less than a period, ingore it
    if (m_resource_list.size() < len)
        return false;
#if 0
    unsigned int n = 0, i = 0;
    for (list<ExecutorStat>::reverse_iterator riter = m_resource_list.rbegin();
         riter != m_resource_list.rend() && i < len; ++riter, ++i) {
        LOG(WARNING) << "!!!!!!!!" << riter->used_cpu;
        // compare to cpu usage
        if (cpu_usage <= riter->used_cpu)
            ++n;
    }
    double per = static_cast<double>(n)/len;
    LOG(INFO) << "cpu usage high than " << cpu_usage 
              << ". real per: " << per << "over load per: " << proportion;
    return per >= proportion;
#endif
    // compute average usage
    unsigned int i = 0;
    double sum = 0;
    for (list<ExecutorStat>::reverse_iterator riter = m_resource_list.rbegin();
         riter != m_resource_list.rend() && i < len; ++riter, ++i) {
        LOG(WARNING) << "!!!!!!!!" << riter->used_cpu;
        sum += riter->used_cpu;
    }
    double value = sum / len;
    LOG(INFO) << "cpu usage high than " << cpu_usage 
              << ". real per: " << value << "over load per: " << cpu_usage;
    return value >= cpu_usage;
}

bool ExecutorInMachine::IsIdle(int period, double value, double proportion) {
    unsigned int len = period/FLAGS_heartbeat_interval;
    ReadLocker locker(m_lock);
    // executor run less than a period, ingore it
    if (m_resource_list.size() < len)
        return false;
    unsigned int n = 0, i = 0;
    for (list<ExecutorStat>::reverse_iterator riter = m_resource_list.rbegin();
         riter != m_resource_list.rend() && i < len; ++riter, ++i) {
        // compare to task_num
        if (value >= riter->task_num)
            ++n;
    }
    double per = static_cast<double>(n)/len;
    LOG(INFO) << "task num lower than " << value
              << ". real per: " << per << "over load per: " << proportion;
    return per >= proportion;
}
