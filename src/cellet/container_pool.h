#ifndef SRC_CELLET_CONTAINER_POOL_H
#define SRC_CELLET_CONTAINER_POOL_H

#include <map>
#include <tr1/functional>
#include <sys/types.h>

#include "common/singleton.h"
#include "common/rwlock.h"
#include "cellet/container.h"

using std::map;
using std::tr1::function;
using std::tr1::placeholders::_1;
using std::tr1::placeholders::_2;
using cello::RWLock;

class ContainerPool {
public:
    typedef function<void(Container*)> ContainerFunc;

    void Insert(const ContainerPtr& ptr);
    void Delete(pid_t pid);

    bool FindToDo(pid_t pid, ContainerFunc func);

    /// @brief: map the container map and do sth
    void MapToDo(ContainerFunc func);

    bool DeleteByTaskId(int64_t task_id);

private:
    RWLock m_lock;
    map<pid_t, ContainerPtr> m_container_pool;
};

typedef Singleton<ContainerPool> ContainerMgr;

#endif
