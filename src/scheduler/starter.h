#ifndef SRC_SCHEDULER_STARTER_H
#define SRC_SCHEDULER_STARTER_H

#include "include/type.h"
#include "scheduler/task.h"
#include "common/rpc.h"
#include "classad/classad.h"
#include "classad/matchClassad.h"

#include "proxy/cellet/gen-cpp/Cellet.h"

class Starter {
public:
    static bool StartTask(const string& endpoint, const TaskPtr& task) {
        shared_ptr<TTransport> transport;
        // get collector proxy
        CelletClient proxy = Rpc<CelletClient, CelletClient>::GetProxy(
                endpoint, TIME_OUT, &transport);
        bool ret = false;
        try {
            transport->open();
            ClassAd task_ad = task->GetClassAd();
            string str_ad = adToString(&task_ad);
            ret = proxy.StartTask(str_ad);
            transport->close();
        } catch (TException &tx) {
            LOG(ERROR) << "start task error: " << tx.what();
        }
        return ret;
    }
};

#endif
