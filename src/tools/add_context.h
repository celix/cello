#ifndef SRC_TOOLS_ADD_CONTEXT_H
#define SRC_TOOLS_ADD_CONTEXT_H

#include "tools/context.h"
#include "proxy/scheduler_wrapper.h"
#include <boost/any.hpp>

using boost::any_cast;

class AddContext : public Context {
public:
    AddContext();
    ~AddContext();

    /// @brief: parse xml configuration file, get configuration
    /// @return: return 0 if success, -1 otherwise
    int Parse(const string& conf_file);

    FrameworkInfoWrapper GetFrameworkInfo() {
        return any_cast<FrameworkInfoWrapper>(m_conf->GetInfo());
    }
};

#endif
