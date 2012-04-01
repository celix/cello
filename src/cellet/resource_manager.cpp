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

MachineInfoWrapper ResourceManager::GetMachineInfo() {
    // get cpu usage
    m_cpu_usage = System::CpuUsage();
    m_avail_memory = m_total_memory;
    m_avail_cpu = m_total_cpu;
    MachineInfoWrapper info(m_endpoint, m_cpu_usage, m_total_cpu, m_total_memory);
    // get every container current resource usage
    // get free cpu and memory
    ContainerPool::ContainerFunc func = bind(&ResourceManager::GetAvailableResource,
                                              this, _1);
    ContainerMgr::Instance()->MapToDo(func);
    func = bind(&ResourceManager::GetExecutorResourceInfo, this, _1, &info,
                m_total_cpu, m_total_memory);
    ContainerMgr::Instance()->MapToDo(func);
    info.SetAvailCpu(m_avail_cpu);
    info.SetAvailMemory(m_avail_memory);
    return info;
}

void ResourceManager::GetAvailableResource(Container* ptr) {
    if (ptr->GetState() == CONTAINER_STARTED) {
        m_avail_cpu -= ptr->GetAllocatedCpu();
        m_avail_memory -= ptr->GetAllocatedMemory();
    }
}

void ResourceManager::GetExecutorResourceInfo(Container* ptr, MachineInfoWrapper* info,
                                       double total_used_cpu, int total_used_memory) {
    if (ptr->GetState() == CONTAINER_STARTED)
        info->AddExecutor(ptr->GetUsedResource(total_used_cpu, total_used_memory));
}

void ResourceManager::SendData() {
    MachineInfoWrapper info = GetMachineInfo();
    MessageQueue::Message msg = info.ToMessage();
    MsgQueueMgr::Instance()->Get(RESOURCE_INFO_KEY)->Send(&msg);
}
