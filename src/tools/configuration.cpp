#include "tools/configuration.h"

any Configuration::Get(const string& attr) {
    map<string, any>::iterator iter = m_attr_map.find(attr);
    if (iter != m_attr_map.end())
        return iter->second;
    return any();
}
