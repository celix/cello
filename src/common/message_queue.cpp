#include <errno.h>
#include <string.h>
#include <string>
#include <stdexcept>

#include "glog/logging.h"
#include "common/message_queue.h"

using std::string;
using std::runtime_error;

MessageQueue::MessageQueue(key_t key): m_key(key), m_id(0) {
    m_id = msgget(m_key, 0666|IPC_CREAT);
    CheckError("MessageQueue::MessageQueue", m_id);
}

MessageQueue::~MessageQueue() {
    if (msgctl(m_id, IPC_RMID, NULL) < 0)
        LOG(ERROR) << "destroy message queue failed with error: " << errno;
}

void MessageQueue::Send(void* data) {
    CheckError("MessageQueue::Send", msgsnd(m_id, data, MAXLEN, 0));
}

void MessageQueue::Receive(void* data) {
    CheckError("MessageQueue::Receive", msgrcv(m_id, data, MAXLEN, 0, 0));
}

void MessageQueue::CheckError(const char* info, int code) {
    if (code < 0) {
        string msg = info;
        msg += " error: ";
        msg += strerror(code);
        throw runtime_error(msg);
    }
}
