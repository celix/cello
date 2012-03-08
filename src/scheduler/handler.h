#ifndef SRC_SCHEDULER_HANDLER_H
#define SRC_SCHEDULER_HANDLER_H

#include "scheduler/event.h"
#include "common/thread.h"
#include <tr1/memory>
#include <tr1/functional>

using cello::Thread;

class Handler {
public:
    Handler() : m_work_thread(0) {}
    ~Handler();

    /// @brief: begin to work
    void Start();

    void Handle();
    void PushBack(const EventPtr& ptr);
private:
    EventQueue m_event_queue;
    Thread* m_work_thread;
};

#endif
