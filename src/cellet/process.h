#ifndef SRC_CELLET_PROCESS_H
#define SRC_CELLET_PROCESS_H

#include "proxy/cellet/gen-cpp/scheduler_types.h"
#include "boost/shared_ptr.hpp"

using boost:shared_ptr;

class Process {
public:
    explicit Process(const TaskInfo info);

    
private:
    int64_t m_task_id;
    string m_cmd;
    string m_arguments;
    string m_framework_name;
    double m_need_cpu;
    double m_need_memory;
};

typedef shared_ptr<Process> ProcessPtr;

#endif
