#ifndef SRC_COLLECTOR_TRIGGER_H
#define SRC_COLLECTOR_TRIGGER_H

#include "boost/shared_ptr.hpp"

using boost::shared_ptr;

class Trigger {
public:
    Trigger(int value, int period) : m_value_threshold(value),
                                     m_period_threshold(period) {}
    Trigger() : m_value_threshold(0), m_period_threshold(0) {}
    virtual ~Trigger() {}
    
    void SetValue(int value) const {
        m_value_threshold = value;
    }
    
    void Setperiod(int period) const {
        m_period_threshold = period;
    }

    /// @brief: return true, then trigger operates, or does nothing
    virtual bool Condition() = 0;

    /// @brief: trigger operation
    virtual bool Operation() = 0;
private:
    int m_value_threshold;    /// value threshold
    int m_period_threshold;     /// period threshold
};

class CpuTrigger : public Trigger {
public:
    CpuTrigger() : Trigger(s_period, s_value) {}
    bool Condition();
    bool Operation();
private:
    static const int s_period = 5; /// In minutes
    static const int s_value = 10; /// In percent    
};

class MemoryTrigger : public Trigger {
public:
    MemoryTrigger(int value, int period) : Trigger(value, period) {}
    bool Condition();
    bool Operation();
};

class SlotTrigger : public Trigger {
    
};

typedef shared_ptr<Trigger> TriggerPtr;

#endif
