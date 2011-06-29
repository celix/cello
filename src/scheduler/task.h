#ifndef SRC_SCHEDULER_TASK_H
#define SRC_SCHEDULER_TASK_H

#include <string>

#include "boost/shared_ptr.hpp"
#include "classad/classad.h"

using std::string;
using boost::shared_ptr;

class Task {
public:

    /// factory method for creating task
    static TASK_PTR GetTask();

pirvate:
    ClassAd m_ad;
    int64_t m_id;
    string m_frame_name;
};

typedef shared_ptr<Task> TASK_PTR;

#endif
