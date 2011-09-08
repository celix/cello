#ifndef SRC_CELLET_RESOURCE_MANAGER
#define SRC_CELLET_RESOURCE_MANAGER

#include <string>
#include <vector>

#include "common/singleton.h"
#include "proxy/collector/gen-cpp/Collector.h"

#include "cellet/container.h"

using std::string;
using std::vector;

class ResourceManager {
public:
    /// @brief: initilize resource information
    void Init();
    
    /// @brief: compute and get current machine information
    MachineInfo GetMachineInfo();
   
    /// @brief: get allocated resource in this machine
    void GetResource(Container* ptr, vector<ExecutorStat>* vector_ptr);
    
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
