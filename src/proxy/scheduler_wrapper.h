#ifndef SRC_PROXY_SCHEDULER_WRAPPER_H
#define SRC_PROXY_SCHEDULER_WRAPPER_H

#include <string>
#include "proxy/scheduler/gen-cpp/scheduler_types.h"

using std::string;

class TaskInfoWrapper {
public:
    TaskInfoWrapper(const string& name, const string& command,
                    const string& arguments, const string& ips,
                    const string& files, double cpu, int memory);

    TaskInfo Get() const {
        return m_info;
    }
private:
    TaskInfo m_info;
};

class FrameworkInfoWrapper {
public:
    FrameworkInfoWrapper(const string& name, int quota, const string& command,
                          const string& arguments, double cpu, int memory);
    FrameworkInfoWrapper(const FrameworkInfo& info): m_info(info) {}
    FrameworkInfo Get() const {
        return m_info;
    }
    
    string GetName() const {
        return m_info.name;
    }
    
    int GetQuota() const {
        return m_info.quota;
    }
    
    TaskInfo GetTaskInfo() const;

    void Dump() const;
    void Log() const;
    
private:
    FrameworkInfo m_info;
};

#endif
