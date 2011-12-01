#ifndef SRC_SCHEDULER_EVENT_H
#define SRC_SCHEDULER_EVENT_H

#include "common/singleton.h"
#include "common/block_queue.h"
#include "boost/shared_ptr.hpp"
#include "scheduler/task.h"


using boost::shared_ptr;
using cello::BlockQueue;

class Event {
public:
    explicit Event(int64_t id, bool status) : m_task_id(id),
                                              m_status(status) {}
    int64_t GetId() const {
        return m_task_id;
    }

    bool GetStatus() const {
        return m_status;
    }

    virtual ~Event() {}
    virtual void Handle() = 0;

private:
    int64_t m_task_id;
    bool m_status;
};

class StartEvent : public Event {
public:
    StartEvent(int64_t id, bool status) : Event(id, status) {}
    void Handle();
};

class FinishEvent : public Event {
public:
    FinishEvent(int64_t id, bool status) : Event(id, status) {}

    void Handle();
};

typedef shared_ptr<Event> EventPtr;
typedef Singleton<BlockQueue<EventPtr> > EventQueue;

#endif
