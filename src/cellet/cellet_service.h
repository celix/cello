#ifndef SRC_CELLET_CELLET_H
#define SRC_CELLET_CELLET_H

#include "proxy/cellet/gen-cpp/Cellet.h"
#include "cellet/executor_pool.h"

class CelletService : public CelletIf {
public:
    bool StartTask(const TaskInfo& info);
    
    int KillTask(int64_t task_id);
   
};

#endif
