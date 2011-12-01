#include <pthread.h>

#include "glog/logging.h"
#include "gflags/gflags.h"

#include "scheduler/scheduler.h"
#include "scheduler/framework_pool.h"
#include "common/rpc.h"

extern void* ScheduleProcessor(void* unused);
extern void* EventProcesseor(void* unused);
extern void* TaskProcessor(void* unused);

DEFINE_int32(port, 10000, "scheduler port");
DEFINE_string(framework_file, "", "the configuration file of framework");
DEFINE_string(collector_endpoint, "", "collector endpoint");

int main(int argc, char ** argv) {

    // set up flags
    if (argc > 1)
        google::ParseCommandLineFlags(&argc, &argv, true);
    else
        google::ReadFromFlagsFile("../conf/scheduler.conf", argv[0], true);
    
    // initilize log
    google::InitGoogleLogging(argv[0]);
    google::SetLogDestination(google::INFO, "../log/scheduler_info_");
    google::SetLogDestination(google::WARNING, "../log/scheduler_warning_");
    google::SetLogDestination(google::ERROR, "../log/scheduler_error_");
    google::SetLogDestination(google::FATAL, "../log/scheduler_fatal_");
    LOG(INFO) << "begin scheduler";
   
    // init framework pool
    if (FrameworkMgr::Instance()->Init(FLAGS_framework_file) < 0) {
        LOG(ERROR) << "read framework file error";
        return -1;
    }
    pthread_t schedule_t, event_t, task_t;
    pthread_create(&schedule_t, NULL, ScheduleProcessor, NULL);
    pthread_create(&event_t, NULL, EventProcesseor, NULL);
    pthread_create(&task_t, NULL, TaskProcessor, NULL);

    Rpc<Scheduler, SchedulerProcessor>::Listen(FLAGS_port);
    
    return 0;
}
