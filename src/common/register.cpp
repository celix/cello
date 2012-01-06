#include "common/register.h"

map<string, ClassFunc> Class::m_fmap;

void* Class::NewInstance(const string& name) {
    map<string, ClassFunc>::iterator it = m_fmap.find(name);
    if (it == m_fmap.end())
        return NULL;
    else
        return it->second();
}

void Class::RegistClass(const string& name, ClassFunc func) {
    m_fmap[name] = func;
}

Register::Register(const string& name, ClassFunc func) {
    Class::RegistClass(name, func);
}
