#include "scheduler/autoscale_pool.h"
#include "scheduler/autoscale_framework.h"

using cello::WriteLocker;
using cello::ReadLocker;

int AutoScalePool::Init(const string& conf_file) {
    return 0;
}

void AutoScalePool::PlugTask(TaskPtr& task) {
    ReadLocker locker(m_lock);
    for (list<Framework*>::iterator it = m_framework_pool.begin();
         it != m_framework_pool.end(); ++it)
        // find the framework
        if (task->GetFrameworkName() == (*it)->GetName())
            (*it)->PlugTask(task, task->GetQueueType());
}

int AutoScalePool::AddFramework(const FrameworkInfoWrapper& info) {
    AutoScaleFramework* fr = new AutoScaleFramework(info);
    // add one executor into framework;
    fr->Init();
    WriteLocker locker(m_lock);
    m_framework_pool.push_back(fr);
    return 0;
}

