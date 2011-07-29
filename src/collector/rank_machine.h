#ifndef SRC_COLLECTOR_RANK_MACHINE_H
#define SRC_COLLECTOR_RANK_MACHINE_H

#include "collector/machine.h"

class Machine;

struct RankMachine {
    Machine* m_ptr;
    double m_rank_value;
    
    RankMachine() : m_ptr(0), m_rank_value(0.0) {}

    Machine* GetMachine() const {
        return m_ptr;
    }

    void SetMachine(Machine* ptr) {
        m_ptr = ptr;
    }

    double GetRankValue() const {
        return m_rank_value;
    }

    void SetRankValue(double value) {
        m_rank_value = value;
    }
};

#endif
