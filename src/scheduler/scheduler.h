#ifndef SRC_SCHEDULER_SCHEDULER_H
#define SRC_SCHEDULER_SCHEDULER_H

#include "proxy/scheduler/gen-cpp/Scheduler.h"

class Scheduler : public SchedulerIf {
public:
    int64_t Submit(const TaskInfo& task_info) {
        return 0;
    }

    void Query(TaskInfo& result, int64_t task_id) {
    }

    int32_t RemoveTask(int64_t task_id) {
        return 0;
    }
};

#endif
