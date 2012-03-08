#include <glog/logging.h>
#include "cellet/cellet_service.h"

bool CelletService::StartTask(const TaskInfo& info) {
    ExecutorPtr ptr(new Executor(info));
    ptr->LogInfo();
    if(ExecutorMgr::Instance()->Find(ptr)) {
        // task had exist in cellet, return false
        return false;
    } else {
        ExecutorMgr::Instance()->Insert(ptr);
        return true;
    }

}
 
int CelletService::KillTask(int64_t task_id) {
    LOG(INFO) << "receive kill task command: id " << task_id;
    ExecutorMgr::Instance()->DeleteExecutor(task_id);
    return 0;
}
