#include "scheduler/state.h"

void State::ChangeState(Task* task, State* t) {
    task->ChangeState(t);
}

void WaitState::TaskAssigned(Task* task) {
    ChangeState(task, TaskRunning::Instance());
}

queue_type WaitState::GetQueueType() {
    return WAIT_QUEUE;
}

queue_type RunState::GetQueueType() {
    return RUN_QUEUE;
}
