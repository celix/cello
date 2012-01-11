
#include <string>

#include "scheduler/task_queue.h"
#include "proxy/scheduler_wrapper.h"
#include "include/type.h"

using std::string;

class Framework {
public:
    Framework(const string& name, int quota) : m_framework_name(name),
                                               m_quota(quota) {}

    Framework(const FrameworkInfoWrapper& info);

    /// @brief: get framework name 
    string GetName() const {
        return m_framework_name;
    }

    /// @brief: add an executor when initilize
    void Init() = 0;

    /// push task into queue
    void PushTask(const TaskPtr& task, queue_type type) = 0;
    
    /// @brief: popfront a task
    bool PopTask(TaskPtr* ptr, queue_type type) = 0;;
    
    /// @brief: remove task by id
    bool RemoveTask(int64_t task_id, queue_type type) = 0;

private:
    string m_framework_name;
    int m_quota;                      // resource quota
};

