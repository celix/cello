#ifndef SRC_COMMON_POOL_H
#define SRC_COMMON_POOL_H

#include <map>
#include <tr1/memory>
#include <tr1/functional>

#include "common/rwlock.h"

using std::map;
using std::tr1::function;
using std::tr1::placeholders::_1;
using std::tr1::placeholders::_2;
using cello::RWLock;
using cello::ReadLocker;
using cello::WriteLocker;

template<typename Key, typename Value>
class Pool {
public:
    typedef function<void(Value value)> PoolFunc;

public:
    /// @brief: Insert 
    virtual void Insert(const Key& key, const Value& value) {
        WriteLocker locker(m_lock);
        m_pool[key] = value;
    }
    
    virtual void Delete(const Key& name) {
        WriteLocker locker(m_lock);
        m_pool.erase(name);
    }

    virtual bool Find(const Key& key) {
        ReadLocker locker(m_lock);
        for (typename map<Key, Value>::iterator it = m_pool.begin();
             it != m_pool.end(); ++it)
            if (key == it->first)
                return true;
        return false;
    }

    virtual bool FindToDo(const Key& key, PoolFunc func) {
        ReadLocker locker(m_lock);
        for (typename map<Key, Value>::iterator it = m_pool.begin();
             it != m_pool.end(); ++it)
            if (key == it->first) {
                func(it->second);
                return true;
            }
        return false;
    }

    virtual void MapToDo(PoolFunc func) {
        ReadLocker locker(m_lock);
        for (typename map<Key, Value>::iterator it = m_pool.begin();
             it != m_pool.end(); ++it)
            func(it->second);
    }

    virtual int Size() {
        ReadLocker locker(m_lock);
        return m_pool.size();
    }

protected:
    RWLock m_lock;
    map<Key, Value> m_pool;
};

#endif
