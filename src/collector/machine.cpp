#include <vector>
#include <glog/logging.h>
#include "collector/machine.h"
#include <classad/classad_distribution.h>
#include "include/attributes.h"
#include "proxy/collector_wrapper.h"

using std::vector;

Machine::Machine(const MachineInfo& info) : MachineInfo(info) {
    m_ad.InsertAttr(ATTR_CPU_USAGE, usage);
    m_ad.InsertAttr(ATTR_CPU, cpu);
    m_ad.InsertAttr(ATTR_MEMORY, memory);
    m_ad.InsertAttr(ATTR_AVAIL_CPU, avail_cpu);
    m_ad.InsertAttr(ATTR_AVAIL_MEMORY, avail_memory);
}

void Machine::LogInfo() const {
    LOG(INFO) << "====================================";
    LOG(INFO) << "Machine information:";
    LOG(INFO) << "Endpoint: " << endpoint;
    LOG(INFO) << "Cpu usage: " << usage;
    LOG(INFO) << "Available cpu cores: " << avail_cpu;
    LOG(INFO) << "Available memory: " << avail_memory;
    LOG(INFO) << "framework resource usage list:";
    for (vector<ExecutorStat>::const_iterator it = executor_list.begin();
        it != executor_list.end(); ++it) {
        ExecutorStatWrapper wrapper(*it);
        LOG(INFO) << wrapper.ToString();
    }
    LOG(INFO) << "====================================";
}

bool Machine::FrameworkExist(const ClassAd* ptr) {
    string framework;
    ptr->EvaluateAttrString(ATTR_FRAMEWORK, framework);
    for (vector<ExecutorStat>::iterator it = executor_list.begin();
        it != executor_list.end(); ++it) {
        if (framework == it->fr_name)
            return true;
    }
    return false;
}

bool Machine::IsMatch(ClassAd* ptr, RankMachine* rank_ptr) {
    // framework has exist in this machine
    if (FrameworkExist(ptr))
        return false;
    ptr->alternateScope = &m_ad;
    bool is_match = false;
    ptr->EvaluateAttrBool(ATTR_TASK_REQUIREMENT, is_match);
    double d_value = 0.0;
    bool b_value;
    // match success
    if (is_match) {
        // check rank value is double
        bool res = ptr->EvaluateAttrNumber(ATTR_TASK_RANK, d_value);
        if(!res) {
            // check rank value is bool
            res = ptr->EvaluateAttrBool(ATTR_TASK_RANK, b_value);
            if (res)
                b_value ? d_value = 1.0 : d_value = 0.0;
            
        }
        LOG(WARNING) << GetEndpoint() << " rank value: " << d_value;
        rank_ptr->SetRankValue(d_value);
        rank_ptr->SetMachine(this);
        return true;
    }
    return false;
}

