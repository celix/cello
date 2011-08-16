#include "gflags/gflags.h"
#include "cellet/resource_manager.h"
#include "cellet/system.h"
#include "cellet/container_pool.h"
#include "cellet/message_manager.h"
#include "common/message_queue.h"
#include "common/get_ip.h"

DECLARE_int32(port);

void ResourceManager::Init() {
    char data[30] = {0};
    snprintf(data, sizeof(data), "%s:%d", GetIP().c_str(), FLAGS_port);
    m_endpoint = data;
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
    m_avail_memory = m_total_memory;
    m_avail_cpu = m_total_cpu;
    // get free cpu and memory
    ContainerPool::ContainerFunc func = bind(&ResourceManager::GetUsedResource,
                                             this, _1);
    ContainerMgr::Instance()->MapToDo(func);
    MachineInfo info;
    info.endpoint = m_endpoint;
    info.usage = m_cpu_usage;
    info.cpu = m_total_cpu;
    info.memory = m_total_memory;
    info.avail_cpu = m_avail_cpu;
    info.avail_memory = m_avail_memory;
    return info;
}

void ResourceManager::GetUsedResource(Container* ptr) {
    if (ptr->GetState() == CONTAINER_STARTED) {
        m_avail_cpu -= ptr->GetUsedCpu();
        m_avail_memory -= ptr->GetUsedMemory();
        DLOG(ERROR) << m_avail_cpu << "||" << m_avail_memory;
    }
}

void ResourceManager::SendData() {
    MachineInfo info = GetMachineInfo();
    MessageQueue::Message msg = ToMessage(info);
    MsgQueueMgr::Instance()->Get(RESOURCE_INFO_KEY)->Send(&msg);
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
