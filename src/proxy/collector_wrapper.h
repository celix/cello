#ifndef SRC_PROXY_COLLECTOR_WRAPPER_H
#define SRC_PROXY_COLLECTOR_WRAPPER_H

#include <string>
#include "proxy/collector/gen-cpp/collector_types.h"
#include "common/message_queue.h"

using std::string;
using cello::MessageQueue;

class ExecutorStatWrapper {
public:
    static const char seperator = '#';
public:
    ExecutorStatWrapper(const ExecutorStat& stat): m_stat(stat) {}
    ExecutorStatWrapper(const string& ss);
    ExecutorStatWrapper(const string& name, int64_t id, double cpu, int mem, int num);
    string GetFramework() const {
        return m_stat.fr_name;
    }
    string ToString() const;
    
    ExecutorStat Get() const {
        return m_stat;
    }
private:
    ExecutorStat m_stat;
};

class MachineInfoWrapper {
public:
    static const char seperator = '\n';
    static const char mc_seperator = '$';
public:
    MachineInfoWrapper(const MessageQueue::Message& msg);
    MachineInfoWrapper(const string& addr, double cpu_usage,
                       int total_cpu, int total_memory);
    MessageQueue::Message ToMessage() const;

    void SetAvailCpu(double cpu) {
        m_info.avail_cpu = cpu;
    }
    
    void SetAvailMemory(int mem) {
        m_info.avail_memory = mem;
    }

    void AddExecutor(const ExecutorStatWrapper& wrapper);

    MachineInfo Get() const {
        return m_info;
    }
private:
    MachineInfo m_info;
};

#endif
