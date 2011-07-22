#include "cellet/message_manager.h"
#include "include/type.h"

void MessageManager::Init() {
    m_queue_map[TASK_START_KEY] = new MessageQueue(TASK_START_KEY);
    m_queue_map[TASK_STATE_KEY] = new MessageQueue(TASK_STATE_KEY);
    m_queue_map[TASK_CONTROL_KEY] = new MessageQueue(TASK_CONTROL_KEY);
    m_queue_map[RESOURCE_INFO_KEY] = new MessageQueue(RESOURCE_INFO_KEY);
}

MessageQueue* MessageManager::Get(key_t key) {
    return m_queue_map[key];
}
