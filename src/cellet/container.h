#ifndef SRC_CELLET_CONTAINER_H
#define SRC_CELLET_CONTAINER_H

#include <sys/types.h>
#include <stdarg.h>
#include <string>
#include <boost/shared_ptr.hpp>

#include "common/message_queue.h"
#include "common/rwlock.h"
#include "include/type.h"
#include "include/proxy.h"
#include "proxy/collector_wrapper.h"

using boost::shared_ptr;
using std::string;
using cello::RWLock;
using cello::MessageQueue;

class Container {
public:
    Container() : m_pid(0),
                             m_c_args(0),
                             m_first(true),
                             m_prev_cpu(0.0),
                             m_prev_total(0.0) {}
    explicit Container(const MessageQueue::Message& msg);
    
    ~Container();

    /// @brief: set up enviroment
    int Init();

    /// @brief: fork process and execute cmd
    int Execute();
    
    /// @brief: 
    MessageQueue::Message ToMessage();
    
    /// @brief: container process finished
    void ContainerFinished();

    /// @brief: container process started
    void ContainerStarted();

    pid_t GetPid() {
        return m_pid;
    }

    int64_t GetId() const {
        return m_info.id;
    }

    double GetAllocatedCpu() {
        return m_info.need_cpu;
    }
    
    int GetAllocatedMemory() {
        return m_info.need_memory;
    }
    /// @brief: get container current used memory and cpu
    ExecutorStatWrapper GetUsedResource(double used_cpu, int used_memory);

    ContainerState GetState();

    uint64_t ParseTime(const char* str);

    /// @brief: stop and recycle the container with all the resources occupied
    int Recycle();
private:
    void RedirectLog();

    /// @brief: set container name
    void SetName();

    /// @brief: set container resource limit
    void SetResourceLimit();

    /// @brief: close all the fd inherited from parent according to /proc/pid/fd
    void CloseInheritedFD();

    /// @brief: get used cpu
    double GetCpuUsage(double used_cpu);

    /// @brief: get used memory
    int GetMemory();

    /// @brief: get executor current task num
    int GetChildrenNum();

    /// @brief: execute the shell command
    int Shell(char* result, const char* format, ...);

    /// @brief: get remote files
    int FetchFiles();
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
    /// is first report resource
    bool m_first;
    /// container cpu time in heartbeat
    double m_prev_cpu;
    /// total cpu time in heartbeat
    double m_prev_total;
};

typedef shared_ptr<Container> ContainerPtr;

#endif
