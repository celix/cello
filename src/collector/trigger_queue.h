#ifndef SRC_COLLECTOR_TRIGGER_QUEUE_H
#define SRC_COLLECTOR_TRIGGER_QUEUE_H

#include <list>
#include <string>
#include <tr1/memory>
#include <tr1/functional>

#include "collector/trigger.h"
#include "collector/framework_in_machine.h"
#include "common/rwlock.h"

using std::list;
using std::string;
using cello::RWLock;
using std::tr1::function;
using std::tr1::placeholders::_1;
using std::tr1::placeholders::_2;

class TriggerQueue {
public:
    void PushBack(const TriggerPtr& trigger);
    
    void PopFront(TriggerPtr* trigger);
    
    bool Erase(const string& name);

    int Size();
    
    void Map(FrameworkInMachine* fim);

    void Flush(FrameworkInMachine* fim);

    void Clear();
private:
    RWLock m_lock;
    list<TriggerPtr> m_list; 
};

#endif
