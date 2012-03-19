#include "collector/trigger_queue.h"

using cello::ReadLocker;
using cello::WriteLocker;

int TriggerQueue::Size() {
    ReadLocker locker(m_lock);
    return m_list.size();
}

void TriggerQueue::Clear() {
    WriteLocker locker(m_lock);
    m_list.clear();
}

void TriggerQueue::PushBack(const TriggerPtr& trigger) {
    WriteLocker locker(m_lock);
    m_list.push_back(trigger);
}

void TriggerQueue::PopFront(TriggerPtr* ptr) {
    WriteLocker locker(m_lock);
    *ptr = m_list.front();
    m_list.pop_front();
}

bool TriggerQueue::Erase(const string& name) {
    WriteLocker locker(m_lock);
    // TODO

#if 0
    for (list<TriggerPtr>::iterator it = m_list.begin(); it != m_list.end(); ++it)
        // find the task
        if (task_id == (*it)->GetId()) {
            m_list.erase(it);
            return true;
        }
#endif
    // not found return false
    return false;
}

void TriggerQueue::MapToDo(TriggerFunc func) {
    ReadLocker locker(m_lock);
    for (list<TriggerPtr>::iterator it = m_list.begin();
         it != m_list.end(); ++it)
        func(it->get());
}

void TriggerQueue::Flush() {
    WriteLocker locker(m_lock);
    for (list<TriggerPtr>::iterator it = m_list.begin();
         it != m_list.end();)
        if ((*it)->IsTriggered() && (*it)->GetName() == "Idle")
            it = m_list.erase(it);
        else
            ++it;
}
