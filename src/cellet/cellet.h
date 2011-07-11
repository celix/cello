#ifndef SRC_CELLET_CELLET_H
#define SRC_CELLET_CELLET_H

#include "proxy/cellet/gen-cpp/Cellet.h"
#include "cellet/process_pool.h"

class Cellet : public CelletIf {
public:
    bool StartTask(const TaskInfo& info) {
        ProcessPtr ptr(new Process(info));
        ptr->LogInfo();
        if(ProcessMgr::Instance()->Find(ptr)) {
            // task had exist in cellet, return false
            return false;
        } else {
            ProcessMgr::Instance()->Insert(ptr);
            return true;
        }
    }
};

#endif
