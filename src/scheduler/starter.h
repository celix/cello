#ifndef SRC_SCHEDULER_STARTER_H
#define SRC_SCHEDULER_STARTER_H

#include "scheduler/task.h"
#include "common/rpc.h"
#include "include/proxy.h"

class Starter {
public:
    static bool StartTask(const string& endpoint, const Task& task) {
        bool ret = false;
        try {
            // get collector proxy
            Proxy<CelletClient> proxy = Rpc<CelletClient, CelletClient>::GetProxy(endpoint);
            ret = proxy().StartTask(task.GetTaskInfo());
        } catch (TException &tx) {
            LOG(ERROR) << "start task error: " << tx.what();
        }
        LOG(INFO) << "StartTask   ID:" << task.GetId();
        return ret;
    }
};

#endif
