#include "gflags/gflags.h"
#include "cellet/resource_manager.h"
#include "cellet/system.h"
#include "cellet/container_pool.h"
#include "common/message_queue.h"

DECLARE_string(port);

void ResourceManager::Init() {
    m_endpoint = GetIP() + ":";
    m_endpoint += FLAGS_port;
    m_total_cpu = System::CpuNum();
    m_total_memory = System::TotalMemory();
    // available cpu initialize as equal to total cpu
    m_avail_cpu = m_total_cpu;
    // available memory initialize as equal to total memory
    m_avail_memory = m_total_memory;
}

MachineInfo ResourceManager::GetMachineInfo() {
    // get cpu usage
    m_cpu_usage = System::CpuUsage();
    // get free cpu and memory
    ContainerMgr::Instance()->MapToDo(ResourceManager::GetUsedResource)
    MachineInfo info;
    info.endpoint = m_endpoint;
    info.usage = m_cpu_usage;
    info.cpu = m_total_cpu;
    info.memory = m_total_memory;
    info.avail_cpu = m_avail_cpu;
    info.avail_memory = m_avail_memory;
    return info;
}

void ResourceManager::GetUsedResource(Conatiner* ptr) {
    if (ptr->GetState() == CONTAINER_STARTED) {
        m_avail_cpu -= ptr->GetNeedCpu();
        m_avail_memory -= ptr->GetNeedMemory();
    }
}

void ResourceManager::SendData() {
    MachineInfo info = GetMachineInfo();
    MessageQueue::Message msg = ToMessage();
    MsgQueueMgr::Instance()->Get(RESOURCE_INFO_KEY)->Send(msg);
}

MessageQueue::Message ResourceManager::ToMessage(const MachineInfo& info) {
    char data[MessageQueue::MAXLEN] = {0};
    // convert executor information into a string with "\n" as separator
    snprintf(data, sizeof(data), "%s\n%f\n%d\n%d\n%f\n%d\n",
            info.endpoint.c_str(), info.usage, info.cpu, info.memory,
            info.avail_cpu, info.avail_memory);
    MessageQueue::Message msg(data);
    return msg;
}
