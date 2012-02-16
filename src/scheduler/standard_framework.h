#ifndef SRC_SCHEDULER_STANDARD_FRAMEWORK_H
#define SRC_SCHEDULER_STANDARD_FRAMEWORK_H

#include "scheduler/framework.h"

class StandardFramework : public Framework {
public:
    StandardFramework(const string& name, int quota) : Framework(name, quota) {}
    StandardFramework(const FrameworkInfoWrapper& info) : Framework(info) {}
};

#endif
