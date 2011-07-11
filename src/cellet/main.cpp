#include "glog/logging.h"
#include "gflags/gflags.h"

#include "cellet/cellet.h"
#include "common/rpc.h"

DEFINE_int32(port, 9998, "cellet port");

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
   
    Rpc<Cellet, CelletProcessor>::Listen(FLAGS_port);
    
    return 0;
}
