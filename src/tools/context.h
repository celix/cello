#ifndef SRC_TOOLS_CONTEXT_H
#define SRC_TOOLS_CONTEXT_H

#include "tools/configuration.h"
#include "common/filesystem.h"

class Context {
public:
    Context(): m_conf(0), m_filesystem(0) {}
    virtual ~Context() {}
    virtual int Parse(const string& config_file) = 0;
    
protected:
    Configuration* m_conf;
    FileSystem* m_filesystem;
};

#endif
