#include "tools/submit_context.h"
#include "tools/task_configuration.h"
#include "common/register.h"

SubmitContext::SubmitContext() {
    // new a configuration
    m_conf = new TaskConfiguration;
    m_conf->Init();
    // get the filesystem according to configuration
    m_filesystem = static_cast<FileSystem* >Class::NewInstance("HdfsFilesystem");
}

SubmitContext::~SubmitContext() {
    delete m_conf;
}

int SubmitContext::Parse() {
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

void SubmitContext::TransferFiles() {
    string files = any_cast<string>(m_conf->Get("transfer_files"));
    if(files.empty()) {
        LOG(INFO) << "no files need to be tranfered";
        return;
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
        
    }
}
