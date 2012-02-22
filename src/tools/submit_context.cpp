#include <gflags/gflags.h>
#include <glog/logging.h>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMNode.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLString.hpp>
#include "tools/submit_context.h"
#include "tools/task_configuration.h"
#include "common/string_utility.h"
#include "include/type.h"
#include "common/register.h"
#include "common/policy.h"
#include "common/xml_handler.h"

#include <vector>

DECLARE_string(dfs_ip);
DECLARE_int32(dfs_port);
using std::vector;
using xercesc::DOMNode;
using xercesc::DOMNodeList;
using xercesc::DOMDocument;
using xercesc::XMLPlatformUtils;
using xercesc::XercesDOMParser;
using xercesc::XMLException;
using xercesc::XMLString;

SubmitContext::SubmitContext() {
    // new a configuration
    m_conf = new TaskConfiguration;
    m_conf->Init();
    // get the filesystem according to configuration
    m_filesystem = static_cast<FileSystem* >(ClassInstance->NewInstance(PolicyMgr::Instance()->Get("FileSystem")));
    // connect remote distribute file system server
    m_filesystem->Connect(FLAGS_dfs_ip, FLAGS_dfs_port);
}

SubmitContext::~SubmitContext() {
    // release all the resources
    delete m_conf;
    m_filesystem->Disconnect();
    delete m_filesystem;
}

int SubmitContext::Parse(const string& conf_file) {
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
    XMLString::transcode("task", str_frame, 19);
    DOMNodeList* node_list = pdocument->getElementsByTagName(str_frame);
    for (unsigned int i = 0; i < node_list->getLength(); ++i) {
        DOMNode* node = node_list->item(i);
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

int SubmitContext::TransferFiles() {
    string files = any_cast<string>(m_conf->Get("transfer_files"));
    if(files.empty()) {
        LOG(INFO) << "no files need to be tranfered";
        return 0;
    }
    LOG(INFO) << "transfer these files ";
    vector<string> vt;
    StringUtility::Split(files, ' ', &vt);
    for (vector<string>::iterator it = vt.begin(); it != vt.end(); ++it) {
        // get filename
        vector<string> tmp;
        StringUtility::Split(*it, '/', &tmp);
        string file_name = tmp[tmp.size() - 1];
        // reset the file path in dfs
        string new_path = DFS_PREFIX + file_name;
        LOG(INFO) << "new file path: " << new_path;
        // copy file from local system to dfs
        if (m_filesystem->CopyFromLocalToDfs(*it,new_path) < 0) {
            LOG(ERROR) << "copy file error: " << *it << " to " << new_path;
            return -1;
        }
    }
    return 0;
}
