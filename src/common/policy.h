#ifndef SRC_COMMON_POLICY_H
#define SRC_COMMON_POLICY_H
#include <map>
#include <string>
#include <xercesc/dom/DOMNode.hpp>
#include <boost/any.hpp>

using std::map;
using std::string;
using xercesc::DOMNode;
using boost::any;

class Policy : public Configuration {
public:
    
    /// @brief: get policy from file
    void GetPolicy(const string& file);
    
    void Init() {}
    
    bool SetValue(DOMNode* node);

    any GetInfo() {
        return any()
    };
    
};

#endif
