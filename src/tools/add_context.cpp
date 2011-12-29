#include "tools/add_context.h"
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLString.hpp>

#include <boost/any.hpp>
#include "tools/framework_configuration.h"

using boost::any;

AddContext::AddContext() {
    m_conf = new FrameworkConfiguraton;
    m_conf->Init();
}

AddContext::~AddContext() {
    delete m_conf;
}

int AddContext::Parse() {
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

FramekworkInfoWrapper AddContext::GetFrameworkInfo() {
    string name = any_cast<string>(m_conf->Get("name"));
    int quota = any_cast<int>(m_conf->Get("quota"));
    string command = any_cast<string>(m_conf->Get("commnad"));
    string arguments = any_cast<string>(m_conf->Get("arguments"));
    double cpu = any_cast<double>(m_conf->Get("cpu"));
    int memory = any_cast<int>(m_conf->Get("memory"));
    FramekworkInfoWrapper wrapper(name, quota, command, arguments, cpu, memory);
    return wrapper;
}
