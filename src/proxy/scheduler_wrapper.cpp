#include "proxy/scheduler_wrapper.h"

TaskInfoWrapper::TaskInfoWrapper(const string& name, const string& command,
                                 const string& arguments, const string& ips,
                                 double cpu, int memory) {
    m_info.framework_name = name;
    m_info.id = -1;
    m_info.cmd = command;
    m_info.arguments = arguments;
    m_info.candidate_ips = ips;
    m_info.need_cpu = cpu;
    m_info.need_memory = memory;
}

FrameworkInfoWrapper::FrameworkInfoWrapper(const string& name, int quota,
                                           const string& command,
                                           const string& arguments,
                                           double cpu, int memory) {
    m_info.name = name;
    m_info.quota = quota;
    m_info.command = command;
    m_info.arguments = arguments;
    m_info.cpu = cpu;
    m_info.memory = memory;
}

TaskInfo FrameworkInfoWrapper::GetTaskInfo() const {
    TaskInfoWrapper info(m_info.name, m_info.command, m_info.arguments,
                         "", m_info.cpu, m_info.memory);
    return info.Get();
}
