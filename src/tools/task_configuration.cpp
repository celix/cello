#include "tools/task_configuration.h"
#include "proxy/scheduler_wrapper.h"
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <glog/logging.h>

using xercesc::XMLString;
using xercesc::DOMNodeList;
using boost::any_cast;

void TaskConfiguration::Init() {
    const char* str_attr[] = {"name","framework", "command", "arguments",
                              "candidate_ips", "transfer_file"};
    const char* int_attr[] = {"memory_limits"};
    const char* double_attr[] = {"cpu_limits"};
    for (size_t i = 0; i < sizeof(str_attr)/sizeof(char*); ++i) {
        string value;
        m_attr_map[str_attr[i]] = value;
    }
    for (size_t i = 0; i < sizeof(int_attr)/sizeof(char*); ++i) {
        int value = 0;
        m_attr_map[int_attr[i]] = value;
    }
    for (size_t i = 0; i < sizeof(double_attr)/sizeof(char*); ++i) {
        double value = 0;
        m_attr_map[double_attr[i]] = value;
    }
}

bool TaskConfiguration::SetValue(DOMNode* node) {
    for(map<string, any>::iterator it = m_attr_map.begin();
        it != m_attr_map.end(); ++it) {
        XMLCh tmp[64];
        XMLString::transcode((it->first).c_str(), tmp, 64);
        if (XMLString::compareString(node->getNodeName(), tmp) == 0) {
            // deal with node without children
            if (node->getChildNodes()->getLength() == 0) {
                char* value = XMLString::transcode(node->getTextContent());
                if (it->second.type() == typeid(string)) {
                    string ss = value;
                    it->second = value;
                } else if (it->second.type() == typeid(int)) {
                    it->second = atoi(value);
                } else if (it->second.type() == typeid(double)) {
                    it->second = atof(value);
                } else {
                    LOG(ERROR) << "invalid type " << it->second.type().name()
                               << " of " << it->first;
                }
                // free memory
                XMLString::release(&value);
                return true;
            } else {
                string tmp;
                // deal with arguments in particular
                DOMNodeList* child_nodes = node->getChildNodes();
                for (unsigned int j = 0; j < child_nodes->getLength(); ++j) {
                    DOMNode* fnode = child_nodes->item(j);
                    char* value = XMLString::transcode(fnode->getTextContent());
                    tmp += value;
                    // space as seperator
                    tmp += " ";
                    XMLString::release(&value);
                }
                it->second = tmp;
                return true;
            }
        }
    }
    return false;
}

any TaskConfiguration::GetInfo() {
    string name = any_cast<string>(m_attr_map["name"]);
    string command = any_cast<string>(m_attr_map["command"]);
    string arguments = any_cast<string>(m_attr_map["arguments"]);
    string ips = any_cast<string>(m_attr_map["candidate_ips"]);
    double cpu = any_cast<double>(m_attr_map["cpu_limits"]);
    int memory = any_cast<int>(m_attr_map["memory_limits"]);
    string files = any_cast<string>(m_attr_map["transfer_file"]);
    TaskInfoWrapper  wrapper(name, command, arguments, ips, files, cpu, memory);
    return wrapper;
}

