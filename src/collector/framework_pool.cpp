#include "collector/framework_pool.h"

void FrameworkPool::Insert(const FrameworkPtr& ptr) {
    m_map[ptr->GetName()] = ptr;
}

void FrameworkPool::Delete(const string& name) {
    m_map.earse(name);
}

void FrameworkPool::FindToDo(const string& name, FrameworkFunc func) {
    for(map<string, FrameworkPtr>::iterator it = m_map.begin();
        it != m_map.end(); ++it)
        if (name == it->first)
            func(it->second);
}
