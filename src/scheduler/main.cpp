#include "glog/logging.h"

int main(int argc, char ** argv) {
    google::InitGoogleLogging(argv[0]);
    google::SetLogDestination(google::INFO, "../log/scheduler_info_");
    google::SetLogDestination(google::WARNING, "../log/scheduler_warning_");
    google::SetLogDestination(google::ERROR, "../log/scheduler_error_");
    google::SetLogDestination(google::FATAL, "../log/scheduler_fatal_");
    LOG(INFO) << "begin scheduler";
    return 0;
}
