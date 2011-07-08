#include "glog/logging.h"
#include "gflags/gflags.h"

#include "collector/collector.h"
#include "common/rpc.h"

DEFINE_int32(port, 9998, "collector port");

int main(int argc, char ** argv) {

    // set up flags
    if (argc > 1)
        google::ParseCommandLineFlags(&argc, &argv, true);
    else
        google::ReadFromFlagsFile("../conf/collector.conf", argv[0], true);
    
    // initilize log
    google::InitGoogleLogging(argv[0]);
    google::SetLogDestination(google::INFO, "../log/collector_info_");
    google::SetLogDestination(google::WARNING, "../log/collector_warning_");
    google::SetLogDestination(google::ERROR, "../log/collector_error_");
    google::SetLogDestination(google::FATAL, "../log/collector_fatal_");
    LOG(INFO) << "begin collector";
   
    Rpc<Collector, CollectorProcessor>::Listen(FLAGS_port);
    
    return 0;
}
