#ifndef SRC_COLLECTOR_MACHINE_H
#define SRC_COLLECTOR_MACHINE_H

#include <list>

#include "glog/logging.h"

#include "proxy/collector/gen-cpp/Collector.h"
#include "classad/classad.h"
#include "common/rwlock.h"
#include "boost/shared_ptr.hpp"

using std::list;
using boost::shared_ptr;

class Machine : public MachineInfo {
public:
    
    explicit Machine(const MachineInfo& info) {
        endpoint = info.endpoint;
        avail_cpu = info.avail_cpu;
        avail_memory = info.avail_memory;
    }
    
    string GetEndpoint() const {
        return endpoint;
    }
    
    ClassAd GetClassAd() const {
        ClassAd ad;
        return ad;
    }
    
    void LogInfo() const {
        LOG(INFO) << "Machine information:";
        LOG(INFO) << "Endpoint: " << endpoint;
        LOG(INFO) << "Available cpu cores: " << avail_cpu;
        LOG(INFO) << "Available memory: " << avail_memory;
    }

    /// @brief: 
    bool IsMatch(ClassAd* condition_ad);
};

typedef shared_ptr<Machine> MachinePtr;

#endif
