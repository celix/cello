#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <string.h>

#include "glog/logging.h"
#include "gflags/gflags.h"

#include "cellet/cellet.h"
#include "cellet/message_manager.h"
#include "cellet/container_pool.h"
#include "cellet/resource_manager.h"
#include "common/rpc.h"

DEFINE_int32(port, 9998, "cellet port");
DEFINE_string(work_directory, "/tmp/cello", "cellet work directory");
DEFINE_string(scheduler_endpoint, "10.5.0.170:9997", "scheduler endpoint");
DEFINE_string(collector_endpoint, "10.5.0.170:9998", "collector endpoint");
DEFINE_string(log, "/tmp/log/cellet", "cellet log path");
DEFINE_string(cello_bin, "/home/chris/cello/bin", "cellet executable path");

extern void* ResourceInfoSender(void* unused);
extern void* ResourceInfoReceiver(void* unused);
extern void* StartExecutorSender(void* unused);
extern void* StartExecutorReceiver(void* unused);
extern void* ExecutorStatusReceiver(void* unused);

void ResourceManagerEntry(int argc, char ** argv) {
    // change process name
    char buf[16] = {0};
    prctl(PR_GET_NAME, buf);
    const char name[] = "-resource";
    strncat(buf, name, strlen(name));
    prctl(PR_SET_NAME, buf);
    LOG(ERROR) << "!!!!!!" << argv[0];
    LOG(INFO) << "resource manager process begin: " << buf;
    ResourceMgr::Instance()->Init();
    // if temperory directory does not exist then create it
    if (access(FLAGS_work_directory.c_str(), F_OK) < 0)
        mkdir(FLAGS_work_directory.c_str(), S_IRWXU|S_IRWXG|S_IROTH);
    pthread_t start_executor_t, resoure_info_t;
    pthread_create(&start_executor_t, NULL, StartExecutorReceiver, NULL);
    pthread_create(&resoure_info_t, NULL, ResourceInfoSender, NULL);

    // wait pid for task execution finished
    pid_t pid;
    int status;
    while (true) {
        if ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
            ContainerPool::ContainerFunc func = bind(&Container::ContainerFinished,
                                                     _1);
            // find the container and deal with the thing
            if(ContainerMgr::Instance()->FindToDo(pid, func))
                // remove the container since it has finished
                ContainerMgr::Instance()->Delete(pid);
        }
        sleep(1);
    }
} 

int main(int argc, char ** argv) {
    // set up flags
    if (argc > 1)
        google::ParseCommandLineFlags(&argc, &argv, true);
    else
        google::ReadFromFlagsFile("../conf/cellet.conf", argv[0], true);
    
    // initilize log
    google::InitGoogleLogging(argv[0]);
    string info_log = FLAGS_log + "/cellet_info_";
    google::SetLogDestination(google::INFO, info_log.c_str());
    string warning_log = FLAGS_log + "/cellet_warning_";
    google::SetLogDestination(google::WARNING, warning_log.c_str());
    string error_log = FLAGS_log + "/cellet_error_";
    google::SetLogDestination(google::ERROR, error_log.c_str());
    string fatal_log = FLAGS_log + "/cellet_fatal_";
    google::SetLogDestination(google::FATAL, fatal_log.c_str());
    LOG(INFO) << "begin cellet";
    // init message queue
    MsgQueueMgr::Instance()->Init();
    pid_t res_manager_pid = fork();
    if (res_manager_pid != 0) {
        LOG(INFO) << "master process begin";
        pthread_t res_info_recv_t, start_exec_t, exec_status_recv_t;
        // start executor thread
        pthread_create(&start_exec_t, NULL, StartExecutorSender, NULL);
        // resouce information 
        pthread_create(&res_info_recv_t, NULL, ResourceInfoReceiver, NULL);
        // receive executor status thread
        pthread_create(&exec_status_recv_t, NULL, ExecutorStatusReceiver, NULL);
        Rpc<Cellet, CelletProcessor>::Listen(FLAGS_port); 
        return 0;
    } else {
        // resource manager process
        ResourceManagerEntry(argc, argv);
    }
}
