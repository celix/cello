#ifndef SRC_SCHEDULER_AUTOSCALE_FRAMEWORK_H
#define SRC_SCHEDULER_AUTOSCALE_FRAMEWORK_H

#include "scheduler/framework.h"

class AutoScaleFramework : public Framework {
public:
    AutoScaleFramework(const FrameworkInfoWrapper& info);

    void Init();

    void PlugTask(TaskPtr& task, queue_type type);

private:
    TaskInfo m_executor_info;
};

#endif
