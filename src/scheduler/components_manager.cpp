#include <assert.h>

#include "scheduler/components_manager.h"
#include "common/register.h"

ComponentsManager::~ComponentsManager() {
    // release resouce
    if (m_pool)
        delete m_pool;
}

void ComponentsManager::CreateComponents(const string& policy_file) {
    // parse policy file
    m_policy.Parse(policy_file);
    // create framework pool
    m_pool = static_cast<FrameworkPool*>(ClassInstance->NewInstance(m_policy.Get("PoolSchema")));
    assert(m_pool);
}
