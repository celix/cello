#ifndef SRC_SCHEDULER_MATCHER_H
#define SRC_SCHEDULER_MATCHER_H

#include "include/type.h"
#include "scheduler/task.h"
#include "common/rpc.h"
#include "classad/classad.h"
#include "classad/matchClassad.h"
#include "gflags/gflags.h"

#include "proxy/collector/gen-cpp/Collector.h"

DECLARE_string(collector_endpoint);

class Matcher {
public:

    /// @brief: get proper machine from collector
    static bool MatchTask(const TaskPtr task, string* endpoint_str) {
        shared_ptr<TTransport> transport;
        // get collector proxy
        CollectorClient proxy = Rpc<CollectorClient, CollectorClient>::GetProxy(
                FLAGS_collector_endpoint, TIME_OUT, &transport);
        bool ret = false;
        try {
            transport->open();
            ClassAd task_ad = task->GetClassAd();
            string str_ad = adToString(&task_ad);
            proxy.Match(*endpoint_str, str_ad);
            transport->close();
            // if the return address is not empty, then return true
            ret = !(*endpoint_str).empty();
        } catch (TException &tx) {
            LOG(ERROR) << "match task to collector error: " << tx.what();
        }
        return ret;
    }
};

#endif
