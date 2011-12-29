#ifndef SRC_TOOLS_CONTEXT_H
#define SRC_TOOLS_CONTEXT_H

#include "tools/configuration.h"
#include "tools/filesystem.h"

class Context {
public:
    virtual ~Context() {}
    virtual int Parse(const string& config_file) = 0;
private:
    Configuration* m_conf;
    FileSystem* m_filesystem;
};

#endif
