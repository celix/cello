#ifndef SRC_SCHEDULER_MATCHER_H
#define SRC_SCHEDULER_MATCHER_H

#include "include/type.h"
#include "scheduler/task.h"
#include "common/rpc.h"
#include <classad/classad.h>
#include <classad/matchClassad.h>
#include <classad/sink.h>
#include <gflags/gflags.h>

#include "include/proxy.h"

DECLARE_string(collector_endpoint);

class Matcher {
public:

    /// @brief: get proper machine from collector
    static bool MatchTask(const Task& task, string* endpoint_str) {
        bool ret = false;
        try {
            ClassAd task_ad = task.GetClassAd();
            ClassAdUnParser unparser;
            string str_ad;
            unparser.Unparse(str_ad, &task_ad);
            //string str_ad = adToString(&task_ad);
            // get collector proxy
            Proxy<CollectorClient> proxy = Rpc<CollectorClient, CollectorClient>::GetProxy(
                    FLAGS_collector_endpoint, TIME_OUT);
            proxy().Match(*endpoint_str, str_ad);
            // if the return address is not empty, then return true
            ret = !(*endpoint_str).empty();
            if (!ret)
                LOG(WARNING) << "match machine failed: " << str_ad;
        } catch (TException &tx) {
            LOG(ERROR) << "match task to collector error: " << tx.what();
        }
        return ret;
    }
};

#endif
