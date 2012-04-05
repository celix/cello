#ifndef SRC_COLLECTOR_FILTER_H
#define SRC_COLLECTOR_FILTER_H

#include <classad/classad_distribution.h>
#include "collector/machine_pool.h"
#include "collector/rank_machine.h"

class Filter {
public:
    static void Filtration(const MachinePtr& machine,
                           const string& task_ad,
                           list<RankMachine>* result) {
        ClassAdParser parser;
        ClassAd* classad_ptr = parser.ParseClassAd(task_ad);
        RankMachine rank_machine;
        if (machine->IsMatch(classad_ptr, &rank_machine))
            result->push_back(rank_machine);
    }

    static bool MachineCompare(const RankMachine& first,
                               const RankMachine& second) {
        return first.GetRankValue() >= second.GetRankValue();
    }
};

#endif
