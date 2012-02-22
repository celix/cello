#include <iostream>
#include <glog/logging.h>
#include "proxy/scheduler_wrapper.h"

using std::cout;
using std::endl;

TaskInfoWrapper::TaskInfoWrapper(const string& name, const string& command,
                                 const string& arguments, const string& ips,
                                 const string& files, double cpu, int memory) {
    m_info.framework_name = name;
    m_info.id = -1;
    m_info.cmd = command;
    m_info.arguments = arguments;
    m_info.candidate_ips = ips;
    m_info.transfer_files = files;
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

void FrameworkInfoWrapper::Dump() const {
    cout << "========================================" << endl;
    cout << "framework information:" << endl;
    cout << "name:           " << m_info.name << endl;
    cout << "quota:          " << m_info.quota << endl;
    cout << "command:        " << m_info.command << endl;
    cout << "arguments:      " << m_info.arguments << endl;
    cout << "cpu limit:      " << m_info.cpu << endl;
    cout << "memory limit:   " << m_info.memory << endl;
    cout << "========================================" << endl;
}

void FrameworkInfoWrapper::Log() const {
    LOG(INFO) << "========================================";
    LOG(INFO) << "framework information:";
    LOG(INFO) << "name:           " << m_info.name;
    LOG(INFO) << "quota:          " << m_info.quota;
    LOG(INFO) << "command:        " << m_info.command;
    LOG(INFO) << "arguments:      " << m_info.arguments;
    LOG(INFO) << "cpu limit:      " << m_info.cpu;
    LOG(INFO) << "memory limit:   " << m_info.memory;
    LOG(INFO) << "========================================";
}

TaskInfo FrameworkInfoWrapper::GetTaskInfo() const {
    TaskInfoWrapper info(m_info.name, m_info.command, m_info.arguments,
                         "", "",  m_info.cpu, m_info.memory);
    return info.Get();
}
