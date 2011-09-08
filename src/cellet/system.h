#ifndef SRC_CELLET_SYS_API_H
#define SRC_CELLET_SYS_API_H

#include <stdint.h>

class System {
public:
    /// @brief: get cpu core number
    static int CpuNum();

    /// @brief: get total physical memory size(MB)
    static int TotalMemory();

    /// @brief: get used memory(MB)
    static int UsedMemory();

    /// @brief: get free physical memory size(MB)
    static int PhysicalMemory();

    /// @brief: get free virtual memory size(MB)
    static int VirtualMemory();

    /// @brief: get cpu average load(5 min)
    static double LoadAvg();

    /// @brief: get cpu usage
    static double CpuUsage();

    /// @brief: get total cpu time in USER_HZ unit
    static uint64_t CpuTime();

    /// @brief: remove the directory
    static void RemoveDir(const char* path);

    /// @brief: get current time
    static void GetCurrentTime(char* str, int len);
};

#endif
