#ifndef SRC_SCHEDULER_FRAMEWORK_H
#define SRC_SCHEDULER_FRAMEWORK_H

#include <string>

#include "scheduler/task_queue.h"

using std::string;

class Framework {
public:
    Framework(const string& name) : m_framework_name(name),
                                    m_quota(0) {}

    /// @brief: get framework name 
    string GetName() const {
        return m_framework_name;
    }

    /// push task into wait queue
    void AddWaitTask(const TaskPtr& task);

private:
    string m_framework_name;
    int m_quota;                      // resource quota
    TaskQueue m_wait_queue;
    TaskQueue m_run_queue;
};

#endif
