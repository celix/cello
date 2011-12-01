#include "common/thread.h"

bool cello::Thread::Start() {
    int ret = pthread_create(&m_id, NULL, Entry, NULL);
    return ret == 0;
}

bool cello::Thread::Join() {
    return pthread_join(m_id, NULL) == 0;
}

void cello::Thread::Entry(void* unused) {
    m_is_running = true;
    m_entry();
    m_is_running = false;
}
