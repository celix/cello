#ifndef SRC_CELLET_RESOURCE_MANAGER
#define SRC_CELLET_RESOURCE_MANAGER

#include <string>
#include <vector>

#include "common/singleton.h"
#include "include/proxy.h"
#include "proxy/collector_wrapper.h"

#include "cellet/container.h"

using std::string;
using std::vector;

class ResourceManager {
public:
    /// @brief: initilize resource information
    void Init();
    
    /// @brief: compute and get current machine information
    MachineInfoWrapper GetMachineInfo();
   
    /// @brief: get available resource in this machine
    void GetAvailableResource(Container* ptr);
    
    /// @brief: get used resource of all the executors in cellet
    void GetExecutorResourceInfo(Container* ptr, MachineInfoWrapper* info,
                                 double total_used_cpu, int total_used_memory);
    void SendData();

    int GetTotalCpus() const {
        return m_total_cpu;
    }

private:
    string m_endpoint;
    int m_total_cpu;
    double m_avail_cpu;
    int m_total_memory;
    int m_avail_memory;
    double m_cpu_usage;
};

typedef Singleton<ResourceManager> ResourceMgr;

#endif
