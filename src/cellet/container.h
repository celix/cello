#ifndef SRC_CELLET_CONTAINER_H
#define SRC_CELLET_CONTAINER_H

#include <sys/types.h>
#include <string>

#include "proxy/cellet/gen-cpp/scheduler_types.h"
#include "common/message_queue.h"
#include "include/type.h"
#include "boost/shared_ptr.hpp"

using boost::shared_ptr;
using std::string;

class Container {
public:
    explicit Container(const MessageQueue::Message& msg);

    /// @brief: set up enviroment
    int Init();

    /// @brief: fork process and execute cmd
    void Execute();
    
    /// @brief: clean the enviroment
    void Clean();
    
    /// @brief: 
    MessageQueue::Message ToMessage();
    
    /// @brief: container process finished
    void ContainerFinished();

    /// @brief: container process started
    void ContainerStarted();

    pid_t GetPid() {
        return m_pid;
    }

    double GetUsedCpu() {
        return m_info.need_cpu;
    }
    
    int GetUsedMem() {
        return m_info.need_memory;
    }

    ContainerState GetState() {
        return m_state;
    }

private:
    pid_t m_pid;
    TaskInfo m_info;
    ContainerState m_state;
    string m_work_diectory;
};

typedef shared_ptr<Container> ContainerPtr;

#endif
