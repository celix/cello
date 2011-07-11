#ifndef SRC_COLLECTOR_COLLECOTR_H
#define SRC_COLLECTOR_COLLECOTR_H


#include "collector/machine_pool.h"
#include "collector/machine.h"
#include "collector/filter.h"

#include "classad/classad.h"
#include "classad/matchClassad.h"

class Collector : public CollectorIf {
public:
    
    void Match(string& result, const string& str_ad) {
        list<MachinePtr> candidate_machine;
        MachinePool::MachineFunc func = bind(&Filter::Filtration, _1, str_ad,
                                             &candidate_machine);
        Pool::Instance()->MapToDo(func);
        // TODO: @chenjing
        // compute the rank for candidate machine and choose the best one
        if (candidate_machine.size() > 0)
            result = candidate_machine.front()->GetEndpoint();
    }

    void Heartbeat(const MachineInfo& info) {
        MachinePtr ptr(new Machine(info));
        Pool::Instance()->Insert(ptr);
    }

};

#endif
