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

int FrameworkPool::Init(const string& conf_file) {
    try {
        XMLPlatformUtils::Initialize();
    } catch(const XMLException& ex) {
        LOG(INFO) << "XML initialization error";
        return -1;
    }
    XercesDOMParser* parser = new XercesDOMParser();
    parser->parse(conf_file.c_str());
    DOMDocument* pdocument = parser->getDocument();
    XMLCh str_name[10], str_quota[10], str_frame[20];
    XMLString::transcode("name", str_name, 9);
    XMLString::transcode("quota", str_quota, 9);
    // get framework
    XMLString::transcode("framework", str_frame, 19);
    DOMNodeList* node_list = pdocument->getElementsByTagName(str_frame);
    for (unsigned int i = 0; i < node_list->getLength(); ++i) {
        DOMNode* node = node_list->item(i);
        DOMNodeList* child_nodes = node->getChildNodes();
        string framework_name;
        int quota;
        for (unsigned int j = 0; j < child_nodes->getLength(); ++j) {
            DOMNode* fnode = child_nodes->item(j);
            // get framework name
            if(XMLString::compareString(fnode->getNodeName(), str_name) == 0) {
                char* value = XMLString::transcode(fnode->getTextContent());
                framework_name = value;
                XMLString::release(&value);
            } else if (XMLString::compareString(fnode->getNodeName(), str_quota) == 0) {
                // get framework quota
                char* value = XMLString::transcode(fnode->getTextContent());
                quota = atoi(value);
                XMLString::release(&value);
            }
        }
        // create the framework and insert into framework pool
        Framework frame(framework_name, quota);
        m_framework_pool.push_back(frame);
    }
    delete parser;
    XMLPlatformUtils::Terminate();
    return 0;
}

void FrameworkPool::AddTask(const TaskPtr& task) {
    // create the bind function for PushTask
    FrameworkFunc func = bind(&Framework::PushTask, _1, task,
                              task->GetQueueType());
    FindToDo(task->GetFrameworkName(), func);
}

bool FrameworkPool::FindToDo(const string& name, FrameworkFunc func) {
    ReadLocker locker(m_lock);
    for (list<Framework>::iterator it = m_framework_pool.begin();
         it != m_framework_pool.end(); ++it)
        // find the framework
        if (name == it->GetName()) {
            func(&(*it));
            return true;
        }
    return false;
}

TaskPtr FrameworkPool::GetTask() {
    ReadLocker locker(m_lock);
    TaskPtr task;
    for (list<Framework>::iterator it = m_framework_pool.begin();
         it != m_framework_pool.end(); ++it) {
        // find a wait task
        if (it->PopTask(&task, WAIT_QUEUE))
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

int FrameworkPool::AddFramework(const FrameworkInfoWrapper& info) {
    Framework fr(info);
    WriteLocker locker(m_lock);
    m_framework_pool.push_back(fr);
    return 0;
}

bool FrameworkPool::DeleteFramework(const string& name) {
    WriteLocker locker(m_lock);
    for (list<Framework>::iterator it = m_framework_pool.begin();
         it != m_framework_pool.end(); ++it)
        // name is unique
        if (name == it->GetName()) {
            m_framework_pool.erase(it);
            return true;
        }
    return false;
}

bool FrameworkPool::GetFrameworkExecutor(const string& name, TaskInfo* info) {
    FrameworkFunc func = bind(&Framework::GetExecutorInfo, _1, info);
    return FindToDo(name, func);
}
