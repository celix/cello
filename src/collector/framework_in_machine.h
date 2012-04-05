#ifndef SRC_COLLECTOR_FRAMEWORK_IN_MACHINE_H
#define SRC_COLLECTOR_FRAMEWORK_IN_MACHINE_H

#include <map>
#include <string>
#include "common/rwlock.h"
#include "collector/executor_in_machine.h"

using std::map;
using std::string;
using cello::RWLock;

class FrameworkInMachine {
public:
    explicit FrameworkInMachine(const string& name) : m_framework_name(name) {}

    string GetName() const {
        return m_framework_name;
    }

    void Insert(ExecutorInMachine* ptr);
    bool Find(const string& address);
    void Delete(const string& address);
    void Delete(int64_t id);
    int Size();
    void Push(const ExecutorStat& stat);

    bool IsOverLoad(int period, double cpu_usage, double proportion);
    bool IsIdle(int period, double value, double proportion, int64_t task_id);
private:
    string m_framework_name;
    RWLock m_lock;
    map<string, ExecutorInMachine*> m_map;
};

#endif
