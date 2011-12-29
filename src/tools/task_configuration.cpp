#include "tools/task_configuration.h"
#include <xercesc/util/XMLString.hpp>

using xercesc::XMLString;

void TaskConfiguration::Init() {
    const char* str_attr[] = {"name","framework", "command", "arguments", "transfer_file"};
    const char* int_attr[] = {"memory_limits"};
    const char* double_attr[] = {"cpu_limits"};
    for (int i = 0; i < sizeof(str_attr)/sizeof(char*); ++i) {
        string value;
        m_map[str_attr[i]] = value;
    }
    for (int i = 0; i < sizeof(int_attr)/sizeof(char*); ++i) {
        int value = 0;
        m_map[int_attr[i]] = value;
    }
    for (int i = 0; i < sizeof(double_attr)/sizeof(char*); ++i) {
        double value = 0;
        m_map[double_attr[i]] = value;
    }
}

bool TaskConfiguration::SetValue(DOMNode* node) {
    for(map<string, any>::iterator it = m_map.begin();
        it != m_map.end(); ++it) {
        XMLCh tmp[64];
        XMLString::transcode((it->first).c_str(), tmp, 64);
        if (it->first != "arguments" || it->first != "transfer_file") {
            if (XMLString::compareString(node->getNodeName(), tmp) == 0) {
                char* value = XMLString::transcode(node->getTextContent());
                switch (it->second.type()) {
                    case typeid(string):
                        it->second = value;
                        break;
                    case typeid(int):
                        it->second = atoi(value);
                        break;
                    case typeid(double):
                        it->second = atof(value);
                        break;
                    default:
                        LOG(ERROR) << "invalid type " << it->second.type().name()
                            << " of " << it->first;
                }
                // free memory
                XMLString::release(&value);
                return true;
            }
        } else {
            it->second = "";
            // deal with arguments in particular
            DOMNodeList* child_nodes = node->getChildNodes();
            for (unsigned int j = 0; j < child_nodes->getLength(); ++j) {
                DOMNode* fnode = child_nodes->item(j);
                char* value = XMLString::transcode(fnode->getTextContent());
                it->second += value;
                // space as seperator
                it->second += " ";
                XMLString::release(&value);
            }    
        }
    }
    return false;
}

