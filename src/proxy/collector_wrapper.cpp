#include <vector>
#include "proxy/collector_wrapper.h"
#include "common/string_utility.h"

using std::vector;

ExecutorStatWrapper::ExecutorStatWrapper(const string& name, int64_t id,
                                         double cpu, int mem, int num) {

        m_stat.fr_name = name,
        m_stat.task_id = id;
        m_stat.used_cpu = cpu,
        m_stat.used_memory = mem,
        m_stat.task_num = num;
}

ExecutorStatWrapper::ExecutorStatWrapper(const string& ss) {
    vector<string> res;
    StringUtility::Split(ss, seperator, &res);
    m_stat.fr_name = res[0];
    m_stat.task_id = atoll(res[1].c_str());
    m_stat.used_cpu = atof(res[2].c_str());
    m_stat.used_memory = atoi(res[3].c_str());
    m_stat.task_num = atoi(res[4].c_str());

}

string ExecutorStatWrapper::ToString() const {
    char data[256] = {0};
    snprintf(data, sizeof(data), "%s%c%lld%c%f%c%d%c%d", m_stat.fr_name.c_str(),
             seperator, m_stat.task_id, seperator, m_stat.used_cpu, seperator,
             m_stat.used_memory, seperator, m_stat.task_num);
    return data;
}

MachineInfoWrapper::MachineInfoWrapper(const string& addr, double cpu_usage,
                                       int total_cpu, int total_memory) {

        m_info.endpoint = addr,
        m_info.usage = cpu_usage,
        m_info.cpu = total_cpu,
        m_info.memory = total_memory;
}

MachineInfoWrapper::MachineInfoWrapper(const MessageQueue::Message& msg) {
    vector<string> res;
    StringUtility::Split(msg.Get(), seperator, &res);
    m_info.endpoint = res[0];
    m_info.usage = atof(res[1].c_str());
    m_info.cpu = atoi(res[2].c_str());
    m_info.memory = atoi(res[3].c_str());
    m_info.avail_cpu = atof(res[4].c_str());
    m_info.avail_memory = atoi(res[5].c_str());
    // executor stat information
    if (res.size() > 6) {
        vector<string> vs;
        StringUtility::Split(res[6], mc_seperator, &vs);
        for (vector<string>::iterator it = vs.begin(); it != vs.end(); ++it) {
            ExecutorStatWrapper wrapper(*it);
            m_info.executor_list.push_back(wrapper.Get());
        }
    }
}

MessageQueue::Message MachineInfoWrapper::ToMessage() const {
    string ss;
    for (vector<ExecutorStat>::const_iterator it = m_info.executor_list.begin();
         it != m_info.executor_list.end(); ++it) {
        ExecutorStatWrapper wrapper(*it);
        ss += wrapper.ToString() + mc_seperator;
    }
    char data[1024] = {0};
    snprintf(data, sizeof(data), "%s%c%f%c%d%c%d%c%f%c%d%c%s",
             m_info.endpoint.c_str(), seperator, m_info.usage, seperator,
             m_info.cpu, seperator, m_info.memory, seperator, m_info.avail_cpu,
             seperator, m_info.avail_memory, seperator, ss.c_str());
    return data;   
}

void MachineInfoWrapper::AddExecutor(const ExecutorStatWrapper& wrapper) {
    m_info.executor_list.push_back(wrapper.Get());
}
