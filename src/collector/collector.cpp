#include <glog/logging.h>
#include <gflags/gflags.h>
#include <stdio.h>
#include <stdlib.h>

#include "collector/collector_service.h"
#include "collector/monitor.h"
#include "common/rpc.h"
#include "common/policy.h"

DEFINE_int32(port, 9998, "collector port");
DEFINE_int32(heartbeat_interval, 2, "heartbeat interval");
DEFINE_string(scheduler_endpoint, "10.5.0.170:9997", "scheduler endpoint");
DEFINE_string(policy_file, "", "policy configuration file path");

int main(int argc, char ** argv) {
    char* cello_home = getenv("CELLO_HOME");
    if (!cello_home) {
        fprintf(stderr, "environment value CELLO_HOME is not set.\n");
        return -1;
    }

    // set up flags
    if (argc > 1) {
        google::ParseCommandLineFlags(&argc, &argv, true);
    } else {
        string config = cello_home;
        config += "/conf/collector.conf";
        google::ReadFromFlagsFile(config.c_str(), argv[0], true);

    }

    // initilize log
    google::InitGoogleLogging(argv[0]);
    string info_log = cello_home;
    info_log += "/log/collector_info_";
    google::SetLogDestination(google::INFO, info_log.c_str());
    string warning_log = cello_home;
    warning_log += "/log/collector_warning_";
    google::SetLogDestination(google::WARNING, warning_log.c_str());
    string erro_log = cello_home;
    erro_log += "/log/collector_error_";
    google::SetLogDestination(google::ERROR, erro_log.c_str());
    string fatal_log = cello_home;
    fatal_log += "/log/collector_fatal_";
    google::SetLogDestination(google::FATAL, fatal_log.c_str());
    LOG(INFO) << "begin collector";

    // get policy file
    if (PolicyMgr::Instance()->Parse(FLAGS_policy_file) < 0)
        return -1;
   
    if (PolicyMgr::Instance()->Get("PoolSchema") == "AutoScalePool")
        // start the monitor
        MonitorMgr::Instance()->Start();
   
    Rpc<CollectorService, CollectorProcessor>::Listen(FLAGS_port);
    
    return 0;
}
