#ifndef SRC_SCHEDULER_EVENT_H
#define SRC_SCHEDULER_EVENT_H

#include "common/singleton.h"
#include "common/block_queue.h"
#include <boost/shared_ptr.hpp>
#include "scheduler/task.h"
#include "include/type.h"
#include <string>

using std::string;
using boost::shared_ptr;
using cello::BlockQueue;

class Event {
public:
    virtual ~Event() {}
    Event(event_type type) : m_type(type) {}
    virtual void Handle() = 0;
    event_type Type() const {
        return m_type;
    }
private:
    event_type m_type;
};

class StateEvent : public Event {
public:
    virtual ~StateEvent() {}
    explicit StateEvent(int64_t id, bool status)
        : Event(STATE_EVENT), m_task_id(id), m_status(status) {}

    int64_t GetId() const {
        return m_task_id;
    }

    bool GetStatus() const {
        return m_status;
    }

private:
    int64_t m_task_id;
    bool m_status;
};

class StartEvent : public StateEvent {
public:
    StartEvent(int64_t id, bool status) : StateEvent(id, status) {}
    void Handle();
};

class FinishEvent : public StateEvent {
public:
    FinishEvent(int64_t id, bool status) : StateEvent(id, status) {}

    void Handle();
};

class RemoveEvent : public StateEvent {
public:
    RemoveEvent(int64_t id): StateEvent(id, true) {}

    void Handle();
};

class ActionEvent : public Event {
public:
    virtual ~ActionEvent() {}

    ActionEvent(const string& address): Event(ACTION_EVENT), m_endpoint(address) {}
    
    string GetAddress() {
        return m_endpoint;
    }
private:
    string m_endpoint;     //cellet endpoint
};

class KillActionEvent : public ActionEvent {
public:
    KillActionEvent(const string& address, int64_t id) : ActionEvent(address), m_id(id) {}
    void Handle();
private:
    int64_t m_id;
};

class StartActionEvent : public ActionEvent {
public:
    StartActionEvent(const string& address) : ActionEvent(address) {}
    void Handle();
};

typedef shared_ptr<Event> EventPtr;
typedef BlockQueue<EventPtr> EventQueue;

#endif
