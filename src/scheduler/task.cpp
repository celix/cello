#include "scheduler/task.h"
#include "scheduler/identity.h"
#include "scheduler/matcher.h"
#include "scheduler/starter.h"

#include "include/attributes.h"
#include "classad/classad_distribution.h"

Task::Task(const TaskInfo& task_info) {
    m_id = TaskIdentity::Instance()->GetTaskId();
    m_framework_name = task_info.framework_name;
    m_submit_time = time(NULL);
    m_state = TaskWaiting::Instance();
    m_task_info = task_info;
    // set task id
    m_task_info.id = m_id;
    // create classad
    m_ad.InsertAttr(ATTR_NEED_CPU, task_info.need_cpu);
    m_ad.InsertAttr(ATTR_NEED_MEMORY, task_info.need_memory);
    // add the rank
    ClassAdParser parser;
    ExprTree* expr = parser.ParseExpression("1-" + ATTR_CPU_USAGE);
    m_ad.Insert(ATTR_TASK_RANK, expr);
    // add the requirement
    ExprTree* re_expr = parser.ParseExpression(
            ATTR_NEED_CPU + "<" + ATTR_AVAIL_CPU + " && " +
            ATTR_NEED_MEMORY + "<" + ATTR_AVAIL_MEMORY);
    m_ad.Insert(ATTR_TASK_REQUIREMENT, re_expr);
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
