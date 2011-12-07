#include <glog/logging.h>

#include "cellet/executor.h"
#include "cellet/message_manager.h"

using cello::WriteLocker;
using cello::ReadLocker;

void Executor::LogInfo() {
    LOG(INFO) << "Executor info";
    LOG(INFO) << "task id: " << m_info.id;
    LOG(INFO) << "cmd: " << m_info.cmd;
    LOG(INFO) << "arguments: " << m_info.arguments;
    LOG(INFO) << "framework: " << m_info.framework_name;
    LOG(INFO) << "cpu requirement: " << m_info.need_cpu;
    LOG(INFO) << "memory requirement: " << m_info.need_memory;
}

void Executor::Start() {
    WriteLocker locker(m_lock);
    // send start information to resource manager process
    MessageQueue::Message msg = ToMessage();
    MsgQueueMgr::Instance()->Get(EXECUTOR_START_KEY)->Send(&msg);
    // change executor state into running
    m_state = EXECUTOR_RUN;
}

ExecutorState Executor::GetStatus() {
    ReadLocker locker(m_lock);
    return m_state;
}

MessageQueue::Message Executor::ToMessage() {
    char data[MessageQueue::MAXLEN] = {0};
    // convert executor information into a string with "\n" as separator
    snprintf(data, sizeof(data), "%lld\n%s\n%s\n%s\n%f\n%d\n", m_info.id,
             m_info.cmd.c_str(), m_info.arguments.c_str(),
             m_info.framework_name.c_str(),
             m_info.need_cpu, m_info.need_memory);
    return data;
}

void Executor::ExecutorStarted() {
    LOG(INFO) << "Executor Start  Framework:" << m_info.framework_name
              << "  Id:" << m_info.id;
    // change state
    WriteLocker locker(m_lock);
    m_state = EXECUTOR_RUN;
}

void Executor::ExecutorFinshed() {
    LOG(INFO) << "Executor Finished  Framework:" << m_info.framework_name
              << "  Id:" << m_info.id;
    WriteLocker locker(m_lock);
    m_state = EXECUTOR_FINISHED;
}
