#ifndef SRC_SCHEDULER_STARTER_H
#define SRC_SCHEDULER_STARTER_H

#include "include/type.h"
#include "scheduler/task.h"
#include "common/rpc.h"

#include "proxy/cellet/gen-cpp/Cellet.h"

class Starter {
public:
    static bool StartTask(const string& endpoint, const Task& task) {
        shared_ptr<TTransport> transport;
        // get collector proxy
        CelletClient proxy = Rpc<CelletClient, CelletClient>::GetProxy(
                endpoint, TIME_OUT, &transport);
        bool ret = false;
        try {
            transport->open();
            ret = proxy.StartTask(task.GetTaskInfo());
            transport->close();
        } catch (TException &tx) {
            LOG(ERROR) << "start task error: " << tx.what();
        }
        LOG(INFO) << "StartTask   ID:" << task.GetId();
        return ret;
    }
};

#endif
