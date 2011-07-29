#ifndef SRC_COLLECTOR_FILTER_H
#define SRC_COLLECTOR_FILTER_H

#include "classad/classad_distribution.h"
#include "collector/machine_pool.h"

class Filter {
public:
    static void Filtration(const MachinePtr& machine,
                           const string& task_ad,
                           list<MachinePtr>* result) {
        ClassAdParser parser;
        ClassAd* classad_ptr = parser.ParseClassAd(task_ad);
        if (machine->IsMatch(classad_ptr))
            result->push_back(machine);

    }
};

#endif
