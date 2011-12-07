#include "common/thread.h"

bool cello::Thread::Start() {
    int ret = pthread_create(&m_id, NULL, Entry, (void*)this);
    return ret == 0;
}

void cello::Thread::DoStart() {
    m_is_running = true;
    m_entry();
}

bool cello::Thread::Join() {
    return pthread_join(m_id, NULL) == 0;
}

void cello::Thread::Terminate() {
    m_is_running = false;
}

void* cello::Thread::Entry(void* in_thread) {
    Thread* th = (Thread* )in_thread;
    th->DoStart();
    th->Terminate();
    return NULL;
}
