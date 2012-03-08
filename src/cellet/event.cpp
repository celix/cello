#include <vector>
#include <string>
#include <glog/logging.h>
#include <stdlib.h>
#include <stdio.h>

#include "cellet/event.h"
#include "cellet/container_pool.h"
#include "common/string_utility.h"

using std::vector;

Event* Event::CreateEvent(const MessageQueue::Message& msg) {
    vector<string> info;
    StringUtility::Split(msg.Get(), '#', &info);
    int64_t id = atol(info[0].c_str());
    event_type type = static_cast<event_type>(atoi(info[1].c_str()));
    switch (type) {
        case KILL_TASK:
            return new KillEvent(id);
            break;
        default:
            return NULL;
    }
}

MessageQueue::Message KillEvent::ToMessage() const {
    char data[100] = {0};
    snprintf(data, sizeof(data), "%lld%c%d", GetId(), seperator, KILL_TASK);
    return data;
}

void KillEvent::Handle() {
    if (ContainerMgr::Instance()->DeleteByTaskId(GetId())) {
        LOG(INFO) << "kill container succeed with id: " << GetId();
    } else {
        LOG(ERROR) << "kill container error: cant find id " << GetId();
    }
}
