#ifndef SRC_TOOLS_TASK_CONFIGURATION_H
#define SRC_TOOLS_TASK_CONFIGURATION_H

#include "tools/configuration.h"

class TaskConfiguration : public Configuration {
public:

    /// @brief: add node for the 
    bool SetValue(DOMNode* node);
    void Init();
    void Dump();
};
                          
#endif
