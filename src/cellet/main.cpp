#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "glog/logging.h"
#include "gflags/gflags.h"

#include "cellet/cellet.h"
#include "cellet/message_manager.h"
#include "common/rpc.h"

DEFINE_int32(port, 9998, "cellet port");
DEFINE_string(work_directory, "/tmp/cello", "cellet work directory");

void ResourceManagerMain() {
    // if temperory directory does not exist then create it
    if (access(FLAGS_work_directory.c_str(), F_OK) < 0)
        mkdir(FLAGS_work_directory.c_str(), S_IRWXU|S_IRWXG|S_IROTH);

} 

int main(int argc, char ** argv) {
    // set up flags
    if (argc > 1)
        google::ParseCommandLineFlags(&argc, &argv, true);
    else
        google::ReadFromFlagsFile("../conf/cellet.conf", argv[0], true);
    
    // initilize log
    google::InitGoogleLogging(argv[0]);
    google::SetLogDestination(google::INFO, "../log/cellet_info_");
    google::SetLogDestination(google::WARNING, "../log/cellet_warning_");
    google::SetLogDestination(google::ERROR, "../log/cellet_error_");
    google::SetLogDestination(google::FATAL, "../log/cellet_fatal_");
    LOG(INFO) << "begin cellet";
    // init message queue
    MsgQueueMgr::Instance()->Init();
    pid_t res_manager_pid = fork();
    if (res_manager_pid != 0) {
        Rpc<Cellet, CelletProcessor>::Listen(FLAGS_port); 
        return 0;
    } else {
        // resource manager process
        ResourceManagerMain();
    }
}
