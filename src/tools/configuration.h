#ifndef SRC_TOOLS_CONFIGURATION_H
#define SRC_TOOLS_CONFIGURATION_H

#include <map>
#include <string>
#include <xercesc/dom/DOMNode.hpp>
#include <boost/any.hpp>

using std::map;
using std::string;
using xercesc::DOMNode;
using boost::any;

class Configuration {
public:
   
    virtual ~Configuration() {}

    /// @brief: add node for the 
    virtual bool SetValue(DOMNode* node) = 0;

    virtual void Init() = 0;

    virtual any GetInfo() = 0;

    any Get(const string& attr);

protected:
    map<string, any> m_attr_map;
};

#endif
