#include "glog/logging.h"

#include "cellet/executor.h"
#include "cellet/message_manager.h"

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
    ReadLocker locker(m_lock);
    // send start information to resource manager process
    MessageQueue::Message msg = ToMessage();
    MsgQueueMgr::Instance()->Get(TASK_START_KEY)->Send(&msg);
}

ExecutorState Executor::GetStatus() {
    ReadLocker locker(m_lock);
    return m_state;
}

MessageQueue::Message Executor::ToMessage() {
    char data[MessageQueue::MAXLEN] = {0};
    // convert executor information into a string with "\n" as separator
    snprintf(data, sizeof(data), "%d\n%s\n%s\n%s\n%f\n%d\n", m_info.id,
             m_info.cmd, m_info.arguments, m_info.framework_name,
             m_info.need_cpu, m_info.need_memory);
    MessageQueue::Message msg(data);
    return msg;
}
