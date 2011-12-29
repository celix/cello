#ifndef SRC_COMMON_REGISTER_H
#define SRC_COMMON_REGISTER_H

typedef void* (*CreateFunc)(void);

class Class {
public:
    static void* NewInstance(const string& name) {
        map<string, CreateFunc>::iterator it = m_cfmap.find(name);
        if (it == m_cfmap.end())
            return NULL;
        else
            return it->second();
    }
    static void RegistClass(const string& name, CreateFunc func) {
        m_cfmap[name] = func;
    }
private:
    static map<string, CreateFunc> m_cfmap;
};

template<class T>
class Register : public Class {
public:
    static void* Instance() {
        return new T;
    }
};

#define REGISTER_CLASS(class_name) \
    string name = #class_name; \
    Register<class_name> class_register; \
    Class::RegistClass(name, class_register::Instance());

#endif
