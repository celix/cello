#ifndef SRC_COMMON_REGISTER_H
#define SRC_COMMON_REGISTER_H

#include <string>
#include <map>
#include "common/singleton.h"

using std::string;
using std::map;

typedef void* (*ClassFunc)();

class Class {
public:
    void* NewInstance(const string& name);
    void RegistClass(const string& name, ClassFunc func);
private:
    map<string, ClassFunc> m_fmap;
};

#define ClassInstance Singleton<Class>::Instance()

class Register {
public:
    Register(const string& name, ClassFunc func);
};

#define REGISTER_CLASS(class_name) \
    class class_name##Register { \
        public: \
            static void* Instance() { \
                return new class_name; \
            } \
        private: \
            static const Register m_register; \
    }; \
    const Register class_name##Register::m_register(#class_name, class_name##Register::Instance);

#endif
