#include "scheduler/handler.h"

using std::tr1::bind;

Handler::~Handler() {
    delete m_work_thread;
}

void Handler::Start() {
    Thread::ThreadFunc func = bind(&Handler::Handle, this);
    m_work_thread = new Thread(func);
    m_work_thread->Start();
}

void Handler::Handle() {
    while (true) {
        EventPtr event;
        m_event_queue.PopFront(&event);
        event->Handle();
    }
}

void Handler::PushBack(const EventPtr& ptr) {
    m_event_queue.PushBack(ptr);
}

