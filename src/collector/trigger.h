#ifndef SRC_COLLECTOR_TRIGGER_H
#define SRC_COLLECTOR_TRIGGER_H

#include <boost/shared_ptr.hpp>
#include "common/singleton.h"
#include "include/proxy.h"
#include "collector/framework_in_machine.h"

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
    
    void Action(FrameworkInMachine* fim);

    /// @brief: return true, then trigger operates, or does nothing
    virtual bool Condition(FrameworkInMachine* fim) = 0;

    /// @brief: trigger operation
    /// @name: framework name
    virtual bool Operation(FrameworkInMachine* fim) = 0;
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
    CpuTrigger(int value = 50, int period = 1) : Trigger("cpu", value, period),
                                                 m_proportion(0.70) {}

    bool Condition(FrameworkInMachine* fim);
    bool Operation(FrameworkInMachine* fim);
private:
    double m_proportion;
};

class MemoryTrigger : public Trigger {
public:
    MemoryTrigger(int value = 10, int period = 5) : Trigger("Memory", value, period) {}
    bool Condition(FrameworkInMachine* fim);
    bool Operation(FrameworkInMachine* fim);
};

class SlotTrigger : public Trigger {
public:
    SlotTrigger(int value = 4, int period = 10) : Trigger("Slot", value, period) {}
    bool Condition(FrameworkInMachine* fim);
    bool Operation(FrameworkInMachine* fim);
};

// every executor correspond to a idle trigger
// 5 min has no task children, then shut down the executor
class IdleTrigger : public Trigger {
public:
    IdleTrigger(int value = 0, int period = 100) : Trigger("Idle", value, period),
                                                 m_id(0) {}
    IdleTrigger(int64_t id, int value = 0, int period = 100)
        : Trigger("Idle", value, period), m_id(id) {}
    bool Condition(FrameworkInMachine* fim);
    bool Operation(FrameworkInMachine* fim);
private:
    int64_t m_id;
};

typedef shared_ptr<Trigger> TriggerPtr;

#endif
