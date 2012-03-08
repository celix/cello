#include "scheduler/dispatcher.h"

void Dispatcher::Register(event_type type, Handler* handler) {
    m_handler_map[type] = handler;
}

Handler* Dispatcher::Dispatch(event_type type) {
    return m_handler_map[type];
}
