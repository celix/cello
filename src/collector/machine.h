#ifndef SRC_COLLECTOR_MACHINE_H
#define SRC_COLLECTOR_MACHINE_H

#include <list>
#include <string>
#include <glog/logging.h>

#include "include/proxy.h"
#include <classad/classad.h>
#include <boost/shared_ptr.hpp>
#include "collector/rank_machine.h"

using std::list;
using std::string;
using boost::shared_ptr;

class Machine : public MachineInfo {
public:
    
    explicit Machine(const MachineInfo& info);
    ~Machine() throw() {}
    string GetEndpoint() const {
        return endpoint;
    }
    
    ClassAd GetClassAd() const {
        return m_ad;
    }
    
    void LogInfo() const;

    /// @brief: 
    bool IsMatch(ClassAd* ptr, RankMachine* rank_ptr);

private:
    /// @brief: check framework exists in the machine
    bool FrameworkExist(const ClassAd* ptr);
private:
    ClassAd m_ad;
};

typedef shared_ptr<Machine> MachinePtr;

#endif
