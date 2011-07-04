#ifndef SRC_SCHEDULER_TASK_H
#define SRC_SCHEDULER_TASK_H

#include <string>

#include "boost/shared_ptr.hpp"
#include "classad/classad.h"
#include "common/singleton.h"
#include "common/block_queue.h"
#include "scheduler/state.h"

#include "proxy/scheduler/gen-cpp/scheduler_types.h"

using std::string;
using boost::shared_ptr;

class Task {
public:
    Task() : m_id(0) {}

    explicit Task(const TaskInfo& task_info);
   
    /// get task id
    int64_t GetId() const {
        return m_id;
    }

    /// get framework name
    string GetFrameworkName() const {
        return m_framework_name;
    }

    /// get resource ad
    ClassAd GetClassAd() const {
        return m_ad;
    }

private:
    ClassAd m_ad;
    int64_t m_id;
    string m_framework_name;
    time_t m_submit_time;
    State* m_state;
};

typedef shared_ptr<Task> TaskPtr;
typedef Singleton< BlockQueue<TaskPtr> > TaskBuffer;

#endif
