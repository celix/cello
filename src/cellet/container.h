#ifndef SRC_CELLET_CONTAINER_H
#define SRC_CELLET_CONTAINER_H

#include <sys/types.h>

#include "proxy/cellet/gen-cpp/scheduler_types.h"
#include "common/message_queue.h"
#include "boost/shared_ptr.hpp"

using boost::shared_ptr;

DECLARE_string(work_directory);

class Container {
public:
    explicit Container(const MessageQueue::Message& msg);
    int Init();
    void Execute();
private:
    pid_t m_pid;
    TaskInfo m_info;
};

typedef shared_ptr<Container> ContainerPtr;

#endif
