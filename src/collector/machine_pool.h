#ifndef SRC_COLLECTOR_MACHINE_POOL_H
#define SRC_COLLECTOR_MACHINE_POOL_H

#include <map>
#include <tr1/memory>
#include <tr1/functional>

#include "common/rwlock.h"
#include "common/singleton.h"

#include "collector/machine.h"

using std::map;
using std::tr1::function;
using std::tr1::placeholders::_1;
using std::tr1::placeholders::_2;
using cello::RWLock;

class MachinePool {
public:
    
    typedef function<void(const MachinePtr&)> MachineFunc;

    /// @brief: insert a machine into pool
    void Insert(const MachinePtr& machine_ptr);

    /// @brief: map a the pool and do func
    void MapToDo(MachineFunc func);

    /// @brief: delete a machine from the pool
    void Delete(const string& endpoint);

    /// @brief: find a element
    bool Find(const string& endpoint);

    /// @brief: map size
    int Size();
private:

    RWLock m_lock;
    map<string, MachinePtr> m_pool;
};

typedef Singleton<MachinePool> Pool;

#endif
