#ifndef SRC_COLLECTOR_EXECUTOR_IN_MACHINE_H
#define SRC_COLLECTOR_EXECUTOR_IN_MACHINE_H

#include <string>
#include <list>
#include "common/rwlock.h"
#include "include/proxy.h"

using std::string;
using std::list;
using cello::RWLock;

class ExecutorInMachine {
public:
    ExecutorInMachine(const string& fr_name, int64_t id, const string& address)
        : m_framework_name(fr_name), m_id(id), m_machine_address(address) {}
    void AddStatInfo(const ExecutorStat& stat);

    int64_t GetId() const {
        return m_id;
    }

    string GetEndpoint() const {
        return m_machine_address;
    }

    /// @brief: decide whether the executor is over load
    /// @period: time period
    /// @cpu_usage: the real cpu is over the cpu_usage, then high load
    /// @proportion: the proportion of high load
    bool IsOverLoad(int period, double cpu_usage, double proportion);

    /// @brief: decide whether the executor is idle
    /// @period: time period
    /// @cpu_usage: the resouce is below the value, then think as idle
    /// @proportion: the proportion of idle
    bool IsIdle(int period, double cpu_usage, double proportion);

private:
    string m_framework_name;
    int64_t m_id;
    string m_machine_address;
    RWLock m_lock;
    list<ExecutorStat> m_resource_list;
};

#endif
