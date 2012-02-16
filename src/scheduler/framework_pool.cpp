#include "scheduler/framework_pool.h"

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLString.hpp>

#include <glog/logging.h>

using cello::ReadLocker;
using cello::WriteLocker;
using namespace xercesc;

void FrameworkPool::AddTask(const TaskPtr& task) {
    // create the bind function for PushTask
    FrameworkFunc func = bind(&Framework::PushTask, _1, task,
                              task->GetQueueType());
    FindToDo(task->GetFrameworkName(), func);
}

bool FrameworkPool::FindToDo(const string& name, FrameworkFunc func) {
    ReadLocker locker(m_lock);
    for (list<Framework*>::iterator it = m_framework_pool.begin();
         it != m_framework_pool.end(); ++it)
        // find the framework
        if (name == (*it)->GetName()) {
            func(*it);
            return true;
        }
    return false;
}

TaskPtr FrameworkPool::GetTask() {
    ReadLocker locker(m_lock);
    TaskPtr task;
    for (list<Framework*>::iterator it = m_framework_pool.begin();
         it != m_framework_pool.end(); ++it) {
        // find a wait task
        if ((*it)->PopTask(&task, WAIT_QUEUE))
            break;
    }
    return task;
}

void FrameworkPool::RemoveTask(const TaskPtr& task) {
    // create the bind function for RemoveTask
    FrameworkFunc func = bind(&Framework::RemoveTask, _1, task->GetId(),
                              task->GetQueueType());
    FindToDo(task->GetFrameworkName(), func);
}

bool FrameworkPool::DeleteFramework(const string& name) {
    WriteLocker locker(m_lock);
    for (list<Framework*>::iterator it = m_framework_pool.begin();
         it != m_framework_pool.end(); ++it)
        // name is unique
        if (name == (*it)->GetName()) {
            delete *it;
            m_framework_pool.erase(it);
            return true;
        }
    return false;
}
