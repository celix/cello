#ifndef SRC_SCHEDULER_SCHEDULER_H
#define SRC_SCHEDULER_SCHEDULER_H

#include "proxy/scheduler/gen-cpp/Scheduler.h"
#include "glog/logging.h"
#include "scheduler/task.h"
#include "scheduler/task_pool.h"
#include "scheduler/event.h"

class Scheduler : public SchedulerIf {
public:
    int64_t Submit(const TaskInfo& task_info) {
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
    void Query(TaskInfo& result, int64_t task_id) {
    }

    /// not support now
    int32_t RemoveTask(int64_t task_id) {
        return 0;
    }

    // @brief: cellet report task start success or fail
    int32_t TaskStarted(int64_t task_id, bool status) {
        LOG(INFO) << "TaskStarted    ID:" << task_id << " STATUS:"
                  << (status ? "success" : "failed");
        // insert into event queue
        EventPtr task_event(new StartEvent(task_id, status));
        EventQueue::Instance()->PushBack(task_event);
        return 0;
    }

    // @brief: cellet report task finish success or fail
    int32_t TaskFinished(int64_t task_id, bool status) {
        LOG(INFO) << "TaskFinished    ID:" << task_id << " STATUS:"
                  << (status ? "success" : "failed");
        // insert into event queue
        EventPtr task_event(new FinishEvent(task_id, status));
        EventQueue::Instance()->PushBack(task_event);
        return 0;
    }

    void LogInfo(const TaskInfo& task_info) {
        LOG(INFO) << "Submit Task:";
        LOG(INFO) << "framework name: " << task_info.framework_name;
        LOG(INFO) << "cmd: " << task_info.cmd;
        LOG(INFO) << "arguments: " << task_info.arguments;
        LOG(INFO) << "candidate ips: " << task_info.candidate_ips;
        LOG(INFO) << "Cpu needs: " << task_info.cpu;
        LOG(INFO) << "Memory needs: " << task_info.memory;
    }
};

#endif
