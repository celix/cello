#ifndef SRC_SCHEDULER_COMPONENTS_MANAGER_H
#define SRC_SCHEDULER_COMPONENTS_MANAGER_H

#include "common/policy.h"
#include "common/singleton.h"
#include "scheduler/framework_pool.h"

class ComponentsManager {
public:
    ComponentsManager() : m_pool(0) {}
    ~ComponentsManager();

    void CreateComponents(const string& policy_file);

    FrameworkPool* GetPool() {
        return m_pool;
    }

private:
    FrameworkPool* m_pool;
    Policy m_policy;
};

typedef Singleton<ComponentsManager> ComponentsMgr;

#endif
