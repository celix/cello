#include <vector>
#include "collector/machine.h"
#include "classad/classad_distribution.h"
#include "include/attributes.h"

using std::vector;

Machine::Machine(const MachineInfo& info) : MachineInfo(info) {
    m_ad.InsertAttr(ATTR_CPU_USAGE, usage);
    m_ad.InsertAttr(ATTR_CPU, cpu);
    m_ad.InsertAttr(ATTR_MEMORY, memory);
    m_ad.InsertAttr(ATTR_AVAIL_CPU, avail_cpu);
    m_ad.InsertAttr(ATTR_AVAIL_MEMORY, avail_memory);
}

void Machine::LogInfo() const {
    LOG(INFO) << "Machine information:";
    LOG(INFO) << "Endpoint: " << endpoint;
    LOG(INFO) << "Cpu usage: " << usage;
    LOG(INFO) << "Available cpu cores: " << avail_cpu;
    LOG(INFO) << "Available memory: " << avail_memory;
    LOG(INFO) << "framework resource usage list:";
    for(vector<ExecutorStat>::const_iterator it = executor_list.begin();
        it != executor_list.end(); ++it)
        LOG(INFO) << it->ToString();
}

bool Machine::IsMatch(ClassAd* ptr, RankMachine* rank_ptr) {
    double d_value = 0.0;
    bool b_value;
    ptr->alternateScope = &m_ad;
    bool is_match = false;
    ptr->EvaluateAttrBool(ATTR_TASK_REQUIREMENT, is_match);
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
        rank_ptr->SetRankValue(d_value);
        rank_ptr->SetMachine(this);
        return true;
    }
    return false;
}

