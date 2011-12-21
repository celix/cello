#ifndef SRC_TOOLS_CONFIGURATION_H
#define SRC_TOOLS_CONFIGURATION_H

#include <map>
#include <xercesc/dom/DOMNode.hpp>
#include <boost/any.hpp>

using std::map;
using xercesc::DOMNode;
using boost::any;

class Configuration {
public:
    Configuration();
    
    /// @brief: add node for the 
    bool SetValue(DOMNode* node);
private:
    void Init();
private:
    map<string, any> m_attr_map;
};

#endif
