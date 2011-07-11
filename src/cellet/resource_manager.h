#ifndef SRC_CELLET_RESOURCE_MANAGER
#define SRC_CELLET_RESOURCE_MANAGER

#include <string>

using std::string;

class ResourceManager {

private:
    string m_endpoint;
    int m_total_cpu;
    double m_avail_cpu;
    int m_total_memory;
    int m_avail_memory;
    double m_load;
};

#endif
