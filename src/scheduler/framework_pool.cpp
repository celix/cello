#include "scheduler/framework_pool.h"

void FrameworkPool::AddTask(const TaskPtr& task) {
    // create the bind function for AddWaitTask
    FrameworkFunc func = bind(Framework::AddWaitTask, _1, task);
    FindToDo(task->GetFrameworkName(), func);
}

bool FrameworkPool::FindToDo(const string& name, FrameworkFunc func) {
    ReadLocker locker(m_lock);
    for (list<Framework>::iterator it = m_framework_pool.begin();
         it != m_framework_pool.end(); ++it)
        // find the framework
        if (name == it->GetName()) {
            it->func();
            return true;
        }
    return false;
}
