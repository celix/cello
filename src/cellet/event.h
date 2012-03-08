#ifndef SRC_CELLET_EVENT_H
#define SRC_CELLET_EVENT_H

#include "common/message_queue.h"

using cello::MessageQueue;

class Event {
public:
    enum event_type {
        KILL_TASK
    };
protected:
    static const char seperator = '#';
public:
    Event(int64_t id) : m_id(id) {}
    virtual ~Event() {}
   
    static Event* CreateEvent(const MessageQueue::Message& msg);

    virtual void Handle() = 0;
    virtual MessageQueue::Message ToMessage() const = 0;

    int64_t GetId() const {
        return m_id;
    }
private:
    int64_t m_id;
};

class KillEvent : public Event {
public:
    KillEvent(int64_t id) : Event(id) {}
    
    MessageQueue::Message ToMessage() const;
    void Handle();

};

#endif
