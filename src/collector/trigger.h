#ifndef SRC_COLLECTOR_TRIGGER_H
#define SRC_COLLECTOR_TRIGGER_H

#include <boost/shared_ptr.hpp>
#include "common/singleton.h"
#include "include/proxy.h"

using boost::shared_ptr;

class Trigger {
public:
    Trigger(const string& name, int value, int period) :
        m_name(name),
        m_value_threshold(value),
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
    
    int GetValue() const {
        return m_value_threshold;
    }

    void SetPeriod(int period) {
        m_period_threshold = period;
    }
    
    int GetPeriod() const {
        return m_period_threshold;
    }

    string GetName() const {
        return m_name;
    }
    
    void Action(const ExecutorStat& stat);

    /// @brief: return true, then trigger operates, or does nothing
    virtual bool Condition(const ExecutorStat& stat) = 0;

    /// @brief: trigger operation
    /// @name: framework name
    virtual bool Operation(const ExecutorStat& stat) = 0;
private:
    string m_name;              /// trigger name
    int m_value_threshold;      /// value threshold
    int m_period_threshold;     /// period threshold
protected:
    time_t m_start_time;        /// trigger begin time
    bool m_is_ignited;          /// ignite flag
};

class CpuTrigger : public Trigger {
public:
    CpuTrigger(int value = 10, int period = 5) : Trigger("cpu", value, period) {}
    bool Condition(const ExecutorStat& stat);
    bool Operation(const ExecutorStat& stat);
};

class MemoryTrigger : public Trigger {
public:
    MemoryTrigger(int value = 10, int period = 5) : Trigger("Memory", value, period) {}
    bool Condition(const ExecutorStat& stat);
    bool Operation(const ExecutorStat& stat);
};

class SlotTrigger : public Trigger {
public:
    SlotTrigger(int value = 4, int period = 10) : Trigger("Slot", value, period) {}
    bool Condition(const ExecutorStat& stat);
    bool Operation(const ExecutorStat& stat);
};

// every executor correspond to a idle trigger
// 5 min has no task children, then shut down the executor
class IdleTrigger : public Trigger {
public:
    IdleTrigger(int value = 0, int period = 1) : Trigger("Idle", value, period),
                                                 m_id(0) {}
    IdleTrigger(int64_t id, int value = 0, int period = 1)
        : Trigger("Idle", value, period), m_id(id) {}
    bool Condition(const ExecutorStat& stat);
    bool Operation(const ExecutorStat& stat);
private:
    int64_t m_id;
};

typedef shared_ptr<Trigger> TriggerPtr;

#endif
