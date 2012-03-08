#ifndef SRC_CELLET_EXECUTOR_H
#define SRC_CELLET_EXECUTOR_H

#include <string>
#include "common/rwlock.h"
#include "common/message_queue.h"
#include "include/type.h"
#include "include/proxy.h"
#include <boost/shared_ptr.hpp>

using std::string;
using boost::shared_ptr;
using cello::RWLock;
using cello::MessageQueue;


class Executor {
public:
    explicit Executor(const TaskInfo info) : m_info(info),
                                             m_state(EXECUTOR_WAIT) {}

    int64_t GetId() {
        return m_info.id;
    }

    ExecutorState GetStatus();

    void Start();

    void Kill();

    void LogInfo();

    void ExecutorStarted();
    
    void ExecutorFinshed();

private:

    /// @brief: convert executor info into message for message queue
    MessageQueue::Message ToMessage();
private:
    TaskInfo m_info;
    ExecutorState m_state;
    RWLock m_lock;
};

typedef shared_ptr<Executor> ExecutorPtr;

#endif
