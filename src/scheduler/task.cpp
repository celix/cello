#include "scheduler/task.h"
#include "scheduler/identity.h"

Task::Task(const TaskInfo& task_info) {
    m_id = TaskIdentity::Instance()->GetTaskId();
    m_framework_name = task_info.framework_name;
    // TODO @chenjing
    
}
