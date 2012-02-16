#ifndef SRC_SCHEDULER_STANDARDPOOL_H
#define SRC_SCHEDULER_STANDARDPOOL_H

#include <list>
#include <string>
#include <tr1/memory>
#include <tr1/functional>

#include "scheduler/framework_pool.h"
#include "common/register.h"

class StandardPool : public FrameworkPool {
public:

    /// @brief: read information from xml file, and init all the framwork
    int Init(const string& conf_file);
    
    /// add the task into correspond framework
    void PlugTask(TaskPtr& task);

    int AddFramework(const FrameworkInfoWrapper& info);
};

REGISTER_CLASS(StandardPool);

#endif
