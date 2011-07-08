#ifndef SRC_COLLECTOR_FILTER_H
#define SRC_COLLECTOR_FILTER_H

#include "collector/machine_pool.h"

class Filter {
public:
    static void Filtration(const Machine& machine,
                           const string& task_ad,
                           list<Machine>* result) {
        // TODO: @chenjing
        // classad match
        result->push_back(machine);    
    }
};

#endif
