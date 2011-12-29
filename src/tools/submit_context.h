#ifndef SRC_TOOLS_SUBMIT_CONTEXT_H
#define SRC_TOOLS_SUBMIT_CONTEXT_H

#include "tools/context.h"

class SubmitContext : public Context {
public:
    SubmitContext();
    ~SubmitContext();
    int Parse(const string& config_file);
    
    /// need to transfer files to the distributed file system
    void TransferFiles();
};

#endif
