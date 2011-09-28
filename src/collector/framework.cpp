#include "collector/framework.h"

void Framework::Start() {
    Thread::ThreadFunc func = bind(Framework::Entry, this);
    m_thread.Init(func);
    m_thread.Start();
}

void Framework::Entry() {
    while(m_executor_pool.Size() > 0) {
        MachinePtr machine;
        m_queue.PopFront(&machine);
        // TODO:chenjing
    }
}
