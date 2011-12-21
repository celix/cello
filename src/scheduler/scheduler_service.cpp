#include "scheduler/scheduler_service.h"
#include "scheduler/framework_pool.h"
#include "proxy/scheduler_wrapper.h"

int64_t SchedulerService::Submit(const TaskInfo& task_info) {
    LogInfo(task_info);
    // create task
    TaskPtr task(new Task(task_info));
    if (task != NULL) {
        // insert the task into pool and buffer
        Pool::Instance()->Insert(task);
        TaskBuffer::Instance()->PushBack(task);
        return task->GetId();
    } else {
        return -1;
    }
}

/// not support now
void SchedulerService::Query(TaskInfo& result, int64_t task_id) {
}

/// not support now
int32_t SchedulerService::RemoveTask(int64_t task_id) {
    return 0;
}

// @brief: cellet report task start success or fail
int32_t SchedulerService::TaskStarted(int64_t task_id, bool status) {
    LOG(INFO) << "TaskStarted    ID:" << task_id << " STATUS:"
        << (status ? "success" : "failed");
    // insert into event queue
    EventPtr task_event(new StartEvent(task_id, status));
    EventQueue::Instance()->PushBack(task_event);
    return 0;
}

// @brief: cellet report task finish success or fail
int32_t SchedulerService::TaskFinished(int64_t task_id, bool status) {
    LOG(INFO) << "TaskFinished    ID:" << task_id << " STATUS:"
        << (status ? "success" : "failed");
    // insert into event queue
    EventPtr task_event(new FinishEvent(task_id, status));
    EventQueue::Instance()->PushBack(task_event);
    return 0;
}

void SchedulerService::LogInfo(const TaskInfo& task_info) {
    LOG(INFO) << "Submit Task:";
    LOG(INFO) << "framework name: " << task_info.framework_name;
    LOG(INFO) << "cmd: " << task_info.cmd;
    LOG(INFO) << "arguments: " << task_info.arguments;
    LOG(INFO) << "candidate ips: " << task_info.candidate_ips;
    LOG(INFO) << "Cpu needs: " << task_info.need_cpu;
    LOG(INFO) << "Memory needs: " << task_info.need_memory;
}

int32_t SchedulerService::AddFramework(const FrameworkInfo& info) {
    LOG(INFO) << "add framework:";
    FrameworkInfoWrapper wrapper(info);
    return FrameworkMgr::Instance()->AddFramework(wrapper);
}

bool SchedulerService::DeleteFramework(const string& name) {
    LOG(INFO) << "delete framework with name: " << name;
    return FrameworkMgr::Instance()->DeleteFramework(name);
}

int32_t SchedulerService::AddExecutor(const string& name) {
    LOG(INFO) << "add a executor for a framework " << name;
    TaskInfo task_info;
    // get the framework executor info
    if (!(FrameworkMgr::Instance()->GetFrameworkExecutor(name, &task_info)))
        return -1;
    TaskPtr task(new Task(task_info));
    if (task) {
        // insert the task into pool and buffer
        Pool::Instance()->Insert(task);
        TaskBuffer::Instance()->PushBack(task);
        return 0;
    } else {
        return -1;
    }
}
