#ifndef SRC_SCHEDULER_TASK_H
#define SRC_SCHEDULER_TASK_H

#include <string>

#include "boost/shared_ptr.hpp"
#include "classad/classad.h"
#include "common/singleton.h"
#include "common/block_queue.h"
#include "common/rwlock.h"
#include "scheduler/state.h"

#include "proxy/scheduler/gen-cpp/scheduler_types.h"

using std::string;
using boost::shared_ptr;

class Task {
public:
    Task() : m_id(0) {}

    explicit Task(const TaskInfo& task_info);
    
    /// @brief: task is assigned
    void TaskAssigned();
   
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

    /// @brief: get queue to be inserted from its state
    queue_type GetQueueType();

    /// @brief: assgin the task
    void AssignTask();

private:
    /// @brief: change task state
    void ChangeState(State* state);

private:
    ClassAd m_ad;
    int64_t m_id;
    string m_framework_name;
    time_t m_submit_time;
    State* m_state;
    friend class WaitState;
    friend class RunState;
    friend class FinishState;
    RWLock m_lock;
};

typedef shared_ptr<Task> TaskPtr;
typedef Singleton< BlockQueue<TaskPtr> > TaskBuffer;

#endif