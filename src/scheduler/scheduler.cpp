#include "proxy/scheduler/gen-cpp/Scheduler.h"

class Scheduler : public SchdulerIF {
    int64_t Submit(const TaskInfo& task_info) {
    }

    void Query(TaskInfo& result, int64_t task_id) {
    }

    int32_t RemoveTask(int64_t task_id) {
    }
}
