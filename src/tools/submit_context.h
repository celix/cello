#ifndef SRC_TOOLS_SUBMIT_CONTEXT_H
#define SRC_TOOLS_SUBMIT_CONTEXT_H

#include "tools/context.h"
#include "proxy/scheduler_wrapper.h"
#include <boost/any.hpp>

using boost::any_cast;

class SubmitContext : public Context {
public:
    SubmitContext();
    ~SubmitContext();
    int Parse(const string& config_file);
    
    /// need to transfer files to the distributed file system
    int TransferFiles();
    
    TaskInfoWrapper GetTaskInfo() {
        return any_cast<TaskInfoWrapper>(m_conf->GetInfo());
    }
};

#endif
