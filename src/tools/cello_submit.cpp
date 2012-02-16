#include <gflags/gflags.h>
#include <glog/logging.h>
#include "tools/submit_context.h"
#include "common/rpc.h"
#include "common/policy.h"
#include "include/proxy.h"

DEFINE_string(master, "", "cello master endpoint");
DEFINE_string(dfs_ip, "", "distributed file system server ip");
DEFINE_int32(dfs_port, 0, "distributed file system server port");
DEFINE_string(policy_file, "", "scheduler configuration policy");

void ErrorMsg(char ** argv) {
    fprintf(stderr, "usage: %s -f [path]\n", argv[0]);
    fprintf(stderr, "\t-f: configuration file of framework\n");
}

int main(int argc, char ** argv) {
    string config_file;
    // set up flags
    if (argc == 3) {
        const char* flag = "-f";
        if (strcmp(argv[1], flag) == 0) {
            config_file = argv[2];
        } else {
            ErrorMsg(argv);
            return -1;
        }
    } else {
        ErrorMsg(argv);
        return -1;
    }
    google::ReadFromFlagsFile("../conf/master", argv[0], true);
    SubmitContext context;
    if (context.Parse(config_file) < 0)
        return -1;
    if (context.TransferFiles() < 0)
        return -1;
    // get policy file
    if (PolicyMgr::Instance()->Parse(FLAGS_policy_file) < 0)
        return -1;
    int64_t id;
    try {
        Proxy<SchedulerClient> proxy =
            Rpc<SchedulerClient, SchedulerClient>::GetProxy(FLAGS_master);
        id = proxy().Submit(context.GetTaskInfo().Get());
    } catch (TException &tx) {
        LOG(ERROR) << "Rpc error: submit task error";
    }
    if (id > 0) {
        fprintf(stderr, "submit task success: id %lld\n", id);
        return 0;
    } else {
        fprintf(stderr, "submit task failed\n");
        return -1;
    }
}
