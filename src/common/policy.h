#ifndef SRC_COMMON_POLICY_H
#define SRC_COMMON_POLICY_H

#include <string>
#include <map>
#include "common/singleton.h"

using std::string;
using std::map;

class Policy {
public:
    int Parse(const string& conf_file);
    
    /// @brief: get attr value by name
    string Get(const string& name);
private:
    map<string, string> m_attr_map;    
};

typedef Singleton<Policy> PolicyMgr;

#endif
