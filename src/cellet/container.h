#ifndef SRC_CELLET_CONTAINER_H
#define SRC_CELLET_CONTAINER_H

#include <sys/types.h>
#include <string>

#include "proxy/cellet/gen-cpp/scheduler_types.h"
#include "proxy/collector/gen-cpp/collector_types.h"
#include "common/message_queue.h"
#include "common/rwlock.h"
#include "include/type.h"
#include "boost/shared_ptr.hpp"

using boost::shared_ptr;
using std::string;

class Container {
public:
    explicit Container(const MessageQueue::Message& msg);
    
    ~Container();

    /// @brief: set up enviroment
    int Init();

    /// @brief: fork process and execute cmd
    void Execute();
    
    /// @brief: 
    MessageQueue::Message ToMessage();
    
    /// @brief: container process finished
    void ContainerFinished();

    /// @brief: container process started
    void ContainerStarted();

    pid_t GetPid() {
        return m_pid;
    }

    double GetAllocatedCpu() {
        return m_info.need_cpu;
    }
    
    int GetAllocatedMemory() {
        return m_info.need_memory;
    }
    /// @brief: get container current used memory and cpu
    ExecutorStat GetUsedResource();

    ContainerState GetState();
private:
    void RedirectLog();

    /// @brief: set container name
    void SetName();

    /// @brief: set container resource limit
    void SetResourceLimit();


private:
    static const double DEFAULT_CPU_SHARE = 0.2;
private:
    pid_t m_pid;
    string m_name;                     //container name
    RWLock m_lock;
    TaskInfo m_info;
    ContainerState m_state;
    string m_work_diectory;
    char** m_c_args;
    time_t m_start_time;
};

typedef shared_ptr<Container> ContainerPtr;

#endif
