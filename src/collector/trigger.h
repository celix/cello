#ifndef SRC_COLLECTOR_TRIGGER_H
#define SRC_COLLECTOR_TRIGGER_H

#include <boost/shared_ptr.hpp>
#include "common/singleton.h"
#include "include/proxy.h"

using boost::shared_ptr;

class Trigger {
public:
    Trigger(int value, int period) : m_value_threshold(value),
                                     m_period_threshold(period),
                                     m_start_time(0),
                                     m_is_ignited(false) {}

    Trigger() : m_value_threshold(0),
                m_period_threshold(0),
                m_start_time(0),
                m_is_ignited(false) {}
    
    virtual ~Trigger() {}
    
    void SetValue(int value) {
        m_value_threshold = value;
    }
    
    void SetPeriod(int period) {
        m_period_threshold = period;
    }

    /// @brief: return true, then trigger operates, or does nothing
    virtual bool Condition(const ExecutorStat& stat) = 0;

    /// @brief: trigger operation
    /// @name: framework name
    virtual bool Operation(const string& name) = 0;
protected:
    int m_value_threshold;      /// value threshold
    int m_period_threshold;     /// period threshold
    time_t m_start_time;        /// trigger begin time
    bool m_is_ignited;          /// ignite flag
};

class CpuTrigger : public Trigger {
public:
    CpuTrigger(int period = 5, int value = 10) : Trigger(period, value) {}
    bool Condition(const ExecutorStat& stat);
    bool Operation(const string& name);
};

class MemoryTrigger : public Trigger {
public:
    MemoryTrigger(int value = 5, int period = 10) : Trigger(value, period) {}
    bool Condition(const ExecutorStat& stat);
    bool Operation(const string& name);
};

class SlotTrigger : public Trigger {
public:
    SlotTrigger(int value = 4, int period = 10) : Trigger(value, period) {}
    bool Condition(const ExecutorStat& stat);
    bool Operation(const string& name);
};

typedef shared_ptr<Trigger> TriggerPtr;
typedef Singleton<CpuTrigger> CPUTrigger;
typedef Singleton<MemoryTrigger> MEMTrigger;
typedef Singleton<SlotTrigger> SLOTTrigger;

#endif
