#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLString.hpp>

#include <glog/logging.h>
#include "scheduler/standard_pool.h"
#include "scheduler/standard_framework.h"
#include "common/xml_handler.h"

using cello::ReadLocker;
using cello::WriteLocker;
using namespace xercesc;

int StandardPool::Init(const string& conf_file) {
    if (conf_file.empty())
        return -1;
        
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
        // remove all the empty text node
        XmlHandler::RemoveEmptyTextNode(node);
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
        StandardFramework* frame = new StandardFramework(framework_name, quota);
        m_framework_pool.push_back(frame);
    }
    delete parser;
    XMLPlatformUtils::Terminate();
    return 0;
}

void StandardPool::PlugTask(TaskPtr& task) {
    AddTask(task);
}

int StandardPool::AddFramework(const FrameworkInfoWrapper& info) {
    StandardFramework* fr = new StandardFramework(info);
    // add one executor into framework;
    fr->Init();
    WriteLocker locker(m_lock);
    m_framework_pool.push_back(fr);
    return 0;
}

