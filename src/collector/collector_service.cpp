#include "collector/collector_service.h"

void CollectorService::Match(string& result, const string& str_ad) {
    LOG(INFO) << "Match ClassAd information:";
    LOG(INFO) << str_ad;
    list<RankMachine> candidate_machine;
    MachinePool::MachineFunc func = bind(&Filter::Filtration, _1, str_ad,
            &candidate_machine);
    Pool::Instance()->MapToDo(func);
    if (candidate_machine.size() > 0) {
        // sort the machine list with rank
        candidate_machine.sort(Filter::MachineCompare);
        result = candidate_machine.front().GetMachine()->GetEndpoint();
        LOG(WARNING) << "Match Task Success. Machine: " << result;
    } else {
        LOG(WARNING) << "Match Task Failed";
    }
}

void CollectorService::Heartbeat(const MachineInfo& info) {
    MachinePtr ptr(new Machine(info));
    ptr->LogInfo();
    Pool::Instance()->Insert(ptr);
    // add event to Monitor
    MonitorMgr::Instance()->AddEvent(ptr);
}

