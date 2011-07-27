#include "common/message_queue.h"
#include "common/string_utility.h"
#include "common/rpc.h"
#include "gflags/gflags.h"
#include "cellet/container.h"
#include "cellet/executor_pool.h"
#include "cellet/container_pool.h"
#include "cellet/message_manager.h"
#include "cellet/resource_manager.h"

#include "proxy/scheduler/gen-cpp/Scheduler.h"

DECLARE_string(scheduler_endpoint);

/// @brief: resource manager thread
///         --send resource info to master process
void* ResourceInfoSender(void* unused) {
    while (true) {
        // send information
        ResourceMgr::Instance()->SendData();
        // heartbeat period is 5 sec
        sleep(5);    
    }
    return NULL;
}

void* ResourceInfoReceiver(void* unused) {
    while (true) {
        
    }
    return NULL;
}

/// @brief: master thread
///         --send start task info to resource manager process
void* StartExecutorSender(void* unused) {
    while (true) {
        ExecutorMgr::Instance()->StartExecutor();
        usleep(1000*100);
    }
    return NULL;
}

/// @brief: resource manager thread
///         --recv task start info and spawn process to start task
void* StartExecutorReceiver(void* unused) {
    while (true) {
        MessageQueue::Message msg;
        MsgQueueMgr::Instance()->Get(EXECUTOR_START_KEY)->Receive(&msg);
        ContainerPtr ptr(new Container(msg));
        // create task execute enviroment
        if (ptr->Init() >= 0) {
            ptr->Execute();
            // insert into the container pool
            ContainerMgr::Instance()->Insert(ptr);
        }
    }
}

/// @brief: master thread
///         --recv executor state info
void* ExecutorStatusReceiver(void* unused) {
    while (true) {
        MessageQueue::Message msg;
        MsgQueueMgr::Instance()->Get(EXECUTOR_STATE_KEY)->Receive(&msg);
        // get executor id
        vector<string> info;
        StringUtility::Split(msg.Get(), '\n', &info);
        int64_t executor_id = atol(info[0]);
        ContainerState state = atoi(info[1]);
        ContainerFunc func = bind(&StateHandler, _1, state);
        if (ExecutorMgr::Instance()->FindToDo(executor_id, func)) {
            if (state == CONTAINER_FINISHED)
                ExecutorMgr::Instance()->Delete(executor_id);
        }
    }
}

void StateHandler(const ExecutorPtr& ptr, ContainerState state) {
    assert(state == CONTAINER_FINISHED || state == CONTAINER_STARTED);
    if (state == CONTAINER_STARTED) {
        ptr->ExecutorStarted();
        shared_ptr<TTransport> transport;
        // get scheduler proxy, report task status to scheduler
        SchedulerClient proxy = Rpc<SchedulerClient, SchedulerClient>::GetProxy(
                FLAGS_scheduler_endpoint, TIME_OUT, &transport);
        try {
            transport->open();
            proxy.TaskStarted(ptr->GetId(), true);
            transport->close();
        } catch (TException &tx) {
            LOG(ERROR) << "report executor start error: " << tx.what();
        }
    } else {
        ptr->ExecutorFinshed();
        shared_ptr<TTransport> transport;
        // get scheduler proxy, report task status to scheduler
        SchedulerClient proxy = Rpc<SchedulerClient, SchedulerClient>::GetProxy(
                FLAGS_scheduler_endpoint, TIME_OUT, &transport);
        try {
            transport->open();
            proxy.TaskFinished(ptr->GetId(), true);
            transport->close();
        } catch (TException &tx) {
            LOG(ERROR) << "report executor finished error: " << tx.what();
        }
    }
}
