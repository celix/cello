#include "tools/framework_configuration.h"
#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <glog/logging.h>

using xercesc::XMLString;
using xercesc::DOMNodeList;
using boost::any_cast;

void FrameworkConfiguration::Init() {
    const char* str_attr[] = {"name", "command", "arguments"};
    const char* int_attr[] = {"quota", "memory"};
    const char* double_attr[] = {"cpu"};
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

bool FrameworkConfiguration::SetValue(DOMNode* node) {
    for(map<string, any>::iterator it = m_attr_map.begin();
        it != m_attr_map.end(); ++it) {
        XMLCh tmp[64];
        XMLString::transcode((it->first).c_str(), tmp, 63);
        if (XMLString::compareString(node->getNodeName(), tmp) == 0) {
            // deal with node without children
            if (node->getChildNodes()->getLength() == 1) {
                char* value = XMLString::transcode(node->getTextContent());
                if (it->second.type() == typeid(string)) {
                    string ss = value;
                    it->second = ss;
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
            } else if (node->getChildNodes()->getLength() > 1){
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

any FrameworkConfiguration::GetInfo() {
    string name = any_cast<string>(m_attr_map["name"]);
    int quota = any_cast<int>(m_attr_map["quota"]);
    string command = any_cast<string>(m_attr_map["command"]);
    string arguments = any_cast<string>(m_attr_map["arguments"]);
    double cpu = any_cast<double>(m_attr_map["cpu"]);
    int memory = any_cast<int>(m_attr_map["memory"]);
    FrameworkInfoWrapper wrapper(name, quota, command, arguments, cpu, memory);
    return wrapper;
}
