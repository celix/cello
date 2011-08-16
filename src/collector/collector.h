#ifndef SRC_COLLECTOR_COLLECOTR_H
#define SRC_COLLECTOR_COLLECOTR_H

#include "collector/machine_pool.h"
#include "collector/machine.h"
#include "collector/rank_machine.h"
#include "collector/filter.h"

#include "classad/classad.h"
#include "classad/matchClassad.h"

class Collector : public CollectorIf {
public:
    
    void Match(string& result, const string& str_ad) {
        LOG(INFO) << "Match ClassAd information:";
        LOG(INFO) << str_ad;
        list<RankMachine> candidate_machine;
        MachinePool::MachineFunc func = bind(&Filter::Filtration, _1, str_ad,
                                             &candidate_machine);
        Pool::Instance()->MapToDo(func);
        if (candidate_machine.size() > 0) {
            result = candidate_machine.front().GetMachine()->GetEndpoint();
            LOG(INFO) << "Match Task Success. Machine: " << result;
        } else {
            LOG(WARNING) << "Match Task Failed";
        }
    }

    void Heartbeat(const MachineInfo& info) {
        MachinePtr ptr(new Machine(info));
        ptr->LogInfo();
        Pool::Instance()->Insert(ptr);
    }

};

#endif
