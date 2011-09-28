#include "common/monitor.h"

void Monitor::Entry(void* param) {
    while(true) {
        MachinePtr machine;
        m_queue.PopFront(&machine);
        ParseMachine(machine);
    }
}

void Monitor::ParseMachine(const MachinePtr& machine) {
    
}

void Monitor::AddMachine(const MachinePtr& machine) {
    m_queue.PushBack(machine);
}`
