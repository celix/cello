#ifndef SRC_CELLET_MESSAGE_MANAGER_H
#define SRC_CELLET_MESSAGE_MANAGER_H

#include <map>
#include "common/message_queue.h"
#include "common/singleton.h"

using std::map;
using cello::MessageQueue;

class MessageManager {
public:
    /// @brief: initilize 4 message queue
    void Init();
    
    /// @brief: get message queue by key
    MessageQueue* Get(key_t key);

private:
    map<key_t, MessageQueue*> m_queue_map;
};

typedef Singleton<MessageManager> MsgQueueMgr;

#endif
