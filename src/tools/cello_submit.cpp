#include <gflags/gflags.h>
#include "tools/submit_context.h"
#include "common/rpc.h"

DEFINE_string(master, "", "cello master endpoint");
DEFINE_string(dfs_ip, "", "distributed file system server ip");
DEFINE_int32(dfs_port, "", "distributed file system server port");

void ErrorMsg() {
    fprintf(stderr, "usage: %s -f [path]\n", argv[0]);
    fprintf(stderr, "\t-f: configuration file of framework\n");
}

int main(int argc, char ** argv) {
    string config_file;
    // set up flags
    if (argc == 3) {
        const char* flag = "-f";
        if (strcmp(argv[1], flag1) == 0) {
            config_file = argv[2];
        } else {
            ErrorMsg();
            return -1;
        }
    } else {
        ErrorMsg();
        return -1;
    }
    google::ReadFromFlagsFile("../conf/master", argv[0], true);
    SubmitContext context;
    if (context.Parse(config_file) < 0)
        return -1;
    Proxy<SchedulerClient> proxy =
        Rpc<SchedulerClient, SchedulerClient>::GetProxy(FLAGS_master);
    
    return 0;
}
