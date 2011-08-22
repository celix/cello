#include "common/message_queue.h"
#include "common/string_utility.h"
#include "gflags/gflags.h"
#include "cellet/executor_pool.h"
#include "cellet/container_pool.h"
#include "cellet/message_manager.h"
#include "cellet/resource_manager.h"
#include "cellet/container.h"

#include "proxy/scheduler/gen-cpp/Scheduler.h"
#include "proxy/collector/gen-cpp/Collector.h"
#include "common/rpc.h"

DECLARE_string(scheduler_endpoint);
DECLARE_string(collector_endpoint);

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

void LogInfo(const MachineInfo& info) {
    LOG(INFO) << "Machine Information:";
    LOG(INFO) << "Endpoint: " << info.endpoint;
    LOG(INFO) << "Cpu Usage: " << info.usage;
    LOG(INFO) << "Cpu Num: " << info.cpu;
    LOG(INFO) << "Total Memory: " << info.memory;
    LOG(INFO) << "Available cpu: " << info.avail_cpu;
    LOG(INFO) << "Available memory: " << info.avail_memory;
}

void* ResourceInfoReceiver(void* unused) {
    while (true) {
        MessageQueue::Message msg;
        MsgQueueMgr::Instance()->Get(RESOURCE_INFO_KEY)->Receive(&msg);
        MachineInfo info(msg);
        try {
            // send heartbeat
            //LogInfo(info);        
            shared_ptr<TTransport> transport;
            // get collector proxy, send heartbeat to collector
            CollectorClient proxy = Rpc<CollectorClient, CollectorClient>::GetProxy(
                    FLAGS_collector_endpoint, TIME_OUT, &transport);
            transport->open();
            proxy.Heartbeat(info);
            transport->close();
            LOG(INFO) << "Send heartbeat success";
        } catch (TException &tx) {
            LOG(ERROR) << "send heartbeat error: " << tx.what();
        }
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
        } else {
            // executor init failed report the status to scheduler
            ptr->ContainerFinished();
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
        int64_t executor_id = atol(info[0].c_str());
        ContainerState state = static_cast<ContainerState>(atoi(info[1].c_str()));
        ExecutorPool::ExecutorFunc func = bind(&StateHandler, _1, state);
        if (ExecutorMgr::Instance()->FindToDo(executor_id, func)) {
            if (state == CONTAINER_FINISHED)
                ExecutorMgr::Instance()->Delete(executor_id);
        }
    }
}

