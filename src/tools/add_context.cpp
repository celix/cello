#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLString.hpp>
#include <glog/logging.h>
#include "tools/add_context.h"
#include "tools/framework_configuration.h"
#include "common/xml_handler.h"

using xercesc::DOMNode;
using xercesc::DOMNodeList;
using xercesc::DOMDocument;
using xercesc::XMLPlatformUtils;
using xercesc::XercesDOMParser;
using xercesc::XMLException;
using xercesc::XMLString;

AddContext::AddContext(): Context() {
    m_conf = new FrameworkConfiguration;
    m_conf->Init();
}

AddContext::~AddContext() {
    delete m_conf;
}

int AddContext::Parse(const string& conf_file) {
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
    // get framework
    XMLString::transcode("framework", str_frame, 19);
    DOMNodeList* node_list = pdocument->getElementsByTagName(str_frame);
    for (unsigned int i = 0; i < node_list->getLength(); ++i) {
        DOMNode* node = node_list->item(i);
        // remove all the empty text node
        XmlHandler::RemoveEmptyTextNode(node);
        DOMNodeList* child_nodes = node->getChildNodes();
        for (unsigned int j = 0; j < child_nodes->getLength(); ++j)
            // set configuration attr value
            if (!m_conf->SetValue(child_nodes->item(j)))
                return -1;
    }
    delete parser;
    XMLPlatformUtils::Terminate();
    return 0;
}
