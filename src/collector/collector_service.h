#ifndef SRC_COLLECTOR_COLLECOTR_SERVICE_H
#define SRC_COLLECTOR_COLLECOTR_SERVICE_H

#include "collector/machine_pool.h"
#include "collector/machine.h"
#include "collector/rank_machine.h"
#include "collector/filter.h"
#include "collector/monitor.h"

#include <classad/classad.h>
#include <classad/matchClassad.h>

class CollectorService : public CollectorIf {
public:
    
    void Match(string& result, const string& str_ad);

    void Heartbeat(const MachineInfo& info);

};

#endif
