#include "scheduler/event.h"
#include "scheduler/task.h"

void StartEvent::Process(TaskPtr task) {
    if (status) {
        FrameworkMgr::Instance()->AddTask(task);
    } else {
        LOG
    }
}

void StartEvent::Handle() {
    // task start success
    if (m_status) {
        TaskPool::TaskFunc func = bind(FrameworkPool)
        // find the task
        if (!(Pool::Instance()->FindToDo(m_task_id, &task)))
            LOG(ERROR) << "can't find task: " << m_task_id;
    } else {
        // task start error so delete it
        Pool::Instance()->Delete(m_task_id);
        LOG(INFO) << "TaskEnd   TASKID:" << m_task_id;
    }
}

void FinishEvent::Handle() {

}
