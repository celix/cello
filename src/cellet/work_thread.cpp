#include "common/message_queue.h"
#include "cellet/container.h"
#include "cellet/executor_pool.h"
#include "cellet/container_pool.h"
#include "cellet/message_manager.h"

void* HeartBeatHandler(void* unused) {
    return NULL;
}

/// @brief: master thread
///         --send start task info to resource manager process
void* StartTaskSender(void* unused) {
    while(true) {
        ExecutorMgr::Instance()->StartExecutor();
        usleep(1000*100);
    }
    return NULL;
}

/// @brief: resource manager thread
///         --recv task start info and spawn process to start task
void* StartTaskReceiver(void* unused) {
    while(true) {
        MessageQueue::Message msg;
        MsgQueueMgr::Instance()->Get(TASK_START_KEY)->Receive(&msg);
        ContainerPtr ptr(new Container(msg));
        // create task execute enviroment
        if (ptr->Init() >= 0) {
            ptr->Execute();
            // insert into the container pool
           // ContainerMgr::Instance()->Insert(ptr);
        }
    }
}
