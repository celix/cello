#ifndef SRC_TOOLS_TASK_CONFIGURATION_H
#define SRC_TOOLS_TASK_CONFIGURATION_H

#include "tools/configuration.h"
#include "proxy/scheduler_wrapper.h"

class TaskConfiguration : public Configuration {
public:

    /// @brief: add node for the 
    bool SetValue(DOMNode* node);
    void Init();
    void Dump();
    
    // @brief: get the info 
    any GetInfo();
};
                          
#endif
