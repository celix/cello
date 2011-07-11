#ifndef SRC_SCHEDULER_STATE_H
#define SRC_SCHEDULER_STATE_H

#include "common/singleton.h"
#include "scheduler/task.h"
#include "include/type.h"

class Task;

/// task state class
class State {
public:
    virtual ~State() {}
    
    virtual void TaskAssigned(Task*) {}
    virtual queue_type GetQueueType() = 0;

    /// @brief: change state
    void ChangeState(Task* task, State* t);
};

class WaitState : public State {
public:
    void TaskAssigned(Task* task);
    
    queue_type GetQueueType();
};

class RunState : public State {
public:
    queue_type GetQueueType();
};

class FinishState : public State {
};

typedef Singleton<WaitState> TaskWaiting;
typedef Singleton<RunState> TaskRunning;
typedef Singleton<FinishState> TaskFinished;

#endif
