#ifndef SRC_SCHEDULER_DISPATCHER_H
#define SRC_SCHEDULER_DISPATCHER_H

#include "scheduler/handler.h"
#include "include/type.h"
#include <map>

using std::map;

class Dispatcher {
public:
    void Register(event_type, Handler* handler);
    Handler* Dispatch(event_type type);
private:
    map<event_type, Handler*> m_handler_map;
};

typedef Singleton<Dispatcher> EventDispatcher;

#endif
