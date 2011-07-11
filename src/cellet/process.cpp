#include "glog/logging.h"

#include "cellet/process.h"

Process::Process(const TaskInfo info) {
    m_task_id = info.id;
    m_cmd = info.cmd;
    m_arguments = info.arguments;
    m_framework_name = info.framework_name;
    m_need_cpu = info.need_cpu;
    m_need_memory = info.need_memory;
}

void Process::LogInfo() {
    LOG(INFO) << "Process info";
    LOG(INFO) << "task id: " << m_task_id;
    LOG(INFO) << "cmd: " << m_cmd;
    LOG(INFO) << "arguments: " << m_arguments;
    LOG(INFO) << "framework: " << m_framework_name;
    LOG(INFO) << "cpu requirement: " << m_need_cpu;
    LOG(INFO) << "memory requirement: " << m_need_memory;
}

