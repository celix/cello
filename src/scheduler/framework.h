#ifndef SRC_SCHEDULER_FRAMEWORK_H
#define SRC_SCHEDULER_FRAMEWORK_H

#include <string>

#include "scheduler/task_queue.h"
#include "include/type.h"

using std::string;

class Framework {
public:
    Framework(const string& name, int quota) : m_framework_name(name),
                                               m_quota(quota) {}

    /// @brief: get framework name 
    string GetName() const {
        return m_framework_name;
    }

    /// push task into queue
    void PushTask(const TaskPtr& task, queue_type type);
    
    /// @brief: popfront a task
    bool PopTask(TaskPtr* ptr, queue_type type);
    
    /// @brief: remove task by id
    bool RemoveTask(int64_t task_id, queue_type type);

private:
    string m_framework_name;
    int m_quota;                      // resource quota
    TaskQueue m_wait_queue;
    TaskQueue m_run_queue;
};

#endif
