#include "scheduler/task.h"
#include "scheduler/identity.h"
#include "scheduler/matcher.h"
#include "scheduler/starter.h"

Task::Task(const TaskInfo& task_info) {
    m_id = TaskIdentity::Instance()->GetTaskId();
    m_framework_name = task_info.framework_name;
    m_submit_time = time(NULL);
    m_state = TaskWaiting::Instance();
    m_task_info = task_info;
    // set task id
    m_task_info.id = m_id;
    // TODO @chenjing
    // add 
    
}

void Task::ChangeState(State* state) {
    WriteLocker locker(m_lock);
    m_state = state;
}

void Task::TaskAssigned() {
    return m_state->TaskAssigned(this);
}

bool Task::AssignTask() {
    WriteLocker locker(m_lock);
    string cellet_address;
    // if match task success then start task
    if (Matcher::MatchTask(*this, &cellet_address)) {
        // task start success
        if (Starter::StartTask(cellet_address, *this)) {
            TaskAssigned();
            return true;
        }
        return false;
    }
    return false;
}

queue_type Task::GetQueueType() {
    ReadLocker locker(m_lock);
    return m_state->GetQueueType();
}
