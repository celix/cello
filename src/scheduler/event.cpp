#include "glog/logging.h"

#include "scheduler/event.h"
#include "scheduler/task.h"
#include "scheduler/task_pool.h"
#include "scheduler/framework_pool.h"
#include "scheduler/components_manager.h"

void StartEvent::Handle() {
    // task start success
    if (GetStatus()) {
        TaskPool::TaskFunc func = bind(&FrameworkPool::AddTask,
                                       ComponentsMgr::Instance()->GetPool(), _1);
        // find the task and insert the task into run queue
        if (!(Pool::Instance()->FindToDo(GetId(), func)))
            LOG(ERROR) << "can't find task: " << GetId();
    } else {
        // task start error so delete it
        Pool::Instance()->Delete(GetId());
        LOG(INFO) << "TaskEnd   TASKID:" << GetId();
    }
}

void FinishEvent::Handle() {
    TaskPool::TaskFunc func = bind(&FrameworkPool::RemoveTask,
                                   ComponentsMgr::Instance()->GetPool(), _1);
    if (Pool::Instance()->FindToDo(GetId(), func)) {
        Pool::Instance()->Delete(GetId());
    } else {
        LOG(ERROR) << "cant find task: " << GetId();
    }
}

