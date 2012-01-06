#ifndef SRC_TOOLS_FRAMEWORK_CONFIGURATION_H
#define SRC_TOOLS_FRAMEWORK_CONFIGURATION_H

#include <xercesc/dom/DOMNode.hpp>
#include "tools/configuration.h"
#include "proxy/scheduler_wrapper.h"

using xercesc::DOMNode;

class FrameworkConfiguration : public Configuration {
public:
    
    /// @brief: add node for the 
    bool SetValue(DOMNode* node);
    void Init();
    
    any GetInfo();
};

#endif
