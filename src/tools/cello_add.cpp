#include <gflags/gflags.h>
#include <glog/logging.h>
#include "tools/add_context.h"
#include "common/rpc.h"
#include "include/proxy.h"

DEFINE_string(master, "", "cello master endpoint");

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
    AddContext context;
    if (context.Parse(config_file) < 0)
        return -1;
    int ret = -1;
    // dump framework information
    context.GetFrameworkInfo().Dump();
    try {
        Proxy<SchedulerClient> proxy =
            Rpc<SchedulerClient, SchedulerClient>::GetProxy(FLAGS_master);
        ret = proxy().AddFramework(context.GetFrameworkInfo().Get());
    } catch (TException &tx) {
        LOG(ERROR) << "Rpc error: Add framework error";
    }
    if (ret < 0) {
        LOG(ERROR) << "Add framework error";
        fprintf(stderr, "Add framework failed.\n");
    } else {
        fprintf(stderr, "Add framework succeed.\n");
    }
    return ret;
}

