#ifndef SRC_SCHEDULER_STATE_H
#define SRC_SCHEDULER_STATE_H

#include "common/singleton.h"
#include "scheduler/task.h"

/// task state class
class State {
public:
    virtual ~State();
    
    virtual void TaskAssigned(TaskPtr task) {}

    /// @brief: change state
    void ChangeState(TaskPtr task, State* t) {
        task->ChangeState(t);
    }
};

class WaitState : public State {
public:
    void TaskAssigned(TaskPtr task) {
        ChangeState(task, TaskRunning);
    }
};

class RunState : public State {
    
};

class FinishState : public State {
};

typedef Singleton<WaitState> TaskWaiting;
typedef Singleton<RunState> TaskRunning;
typedef Singleton<FinishState> TaskFinished;

#endif
