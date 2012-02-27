#ifndef SRC_SCHEDULER_SCHEDULER_H
#define SRC_SCHEDULER_SCHEDULER_H

#include "include/proxy.h"
#include "glog/logging.h"
#include "scheduler/task.h"
#include "scheduler/task_pool.h"
#include "scheduler/event.h"

class SchedulerService : public SchedulerIf {
public:
    int64_t Submit(const TaskInfo& task_info);

    /// not support now
    void Query(TaskInfo& result, int64_t task_id);

    /// not support now
    int32_t RemoveTask(int64_t task_id);

    // @brief: cellet report task start success or fail
    int32_t TaskStarted(int64_t task_id, bool status);

    // @brief: cellet report task finish success or fail
    int32_t TaskFinished(int64_t task_id, bool status);

    void LogInfo(const TaskInfo& task_info);
    
    int32_t AddFramework(const FrameworkInfo& info);

    bool DeleteFramework(const string& name);

    /// @brief: add a executor for a framework name
    int32_t AddExecutor(const string& name);
    
    /// @brief: delete an executor
    int32_t DeleteExecutor(int64_t executor_id);
};

#endif
