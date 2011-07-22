#ifndef SRC_CELLET_CELLET_H
#define SRC_CELLET_CELLET_H

#include "proxy/cellet/gen-cpp/Cellet.h"
#include "cellet/executor_pool.h"

class Cellet : public CelletIf {
public:
    bool StartTask(const TaskInfo& info) {
        ExecutorPtr ptr(new Executor(info));
        ptr->LogInfo();
        if(ExecutorMgr::Instance()->Find(ptr)) {
            // task had exist in cellet, return false
            return false;
        } else {
            ExecutorMgr::Instance()->Insert(ptr);
            return true;
        }
    }
};

#endif
