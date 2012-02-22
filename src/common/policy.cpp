#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLString.hpp>
#include <glog/logging.h>

#include "common/policy.h"
#include "common/xml_handler.h"

using xercesc::DOMNode;
using xercesc::DOMNodeList;
using xercesc::DOMDocument;
using xercesc::XMLPlatformUtils;
using xercesc::XercesDOMParser;
using xercesc::XMLException;
using xercesc::XMLString;

int Policy::Parse(const string& conf_file) {
    try {
        XMLPlatformUtils::Initialize();
    } catch(const XMLException& ex) {
        LOG(INFO) << "XML initialization error";
        return -1;
    }
    XercesDOMParser* parser = new XercesDOMParser();
    parser->parse(conf_file.c_str());
    DOMDocument* pdocument = parser->getDocument();
    XMLCh str_frame[20];
    // get property
    XMLString::transcode("property", str_frame, 19);
    DOMNodeList* node_list = pdocument->getElementsByTagName(str_frame);
    for (unsigned int i = 0; i < node_list->getLength(); ++i) {
        DOMNode* node = node_list->item(i);
        // remove all the empty text node
        XmlHandler::RemoveEmptyTextNode(node);
        DOMNodeList* child_nodes = node->getChildNodes();
        string name, value;
        XMLCh str_name[8], str_value[8];
        XMLString::transcode("name", str_name, 7);
        XMLString::transcode("value", str_value, 7);
        for (unsigned int j = 0; j < child_nodes->getLength(); ++j) {
            DOMNode* fnode = child_nodes->item(j);
            // get property name
            if (XMLString::compareString(fnode->getNodeName(), str_name) == 0) {
                char* data = XMLString::transcode(fnode->getTextContent());
                name = data;
                XMLString::release(&data);
            }
            // get property value
            if (XMLString::compareString(fnode->getNodeName(), str_value) == 0) {
                char* data = XMLString::transcode(fnode->getTextContent());
                value = data;
                XMLString::release(&data);
            }
        }
        // insert the property map
        if (!name.empty() && !value.empty())
            m_attr_map[name] = value;
    }
    delete parser;
    XMLPlatformUtils::Terminate();
    return 0;
}

string Policy::Get(const string& name) {
    map<string, string>::iterator it = m_attr_map.find(name);
    if (it != m_attr_map.end())
        return it->second;
    else
        return string();
}
