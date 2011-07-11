#include "cellet/process.h"

Process::Process(const TaskInfo info) {
    m_task_id = info.id;
    m_cmd = info.cmd;
    m_arguments = info.arguments;
    m_framework_name = info.framework_name;
    m_need_cpu = info.need_cpu;
    m_need_memory = info.need_memory;
}
