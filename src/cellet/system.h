#ifndef SRC_CELLET_SYS_API_H
#define SRC_CELLET_SYS_API_H

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

    /// @brief: remove the directory
    static void RemoveDir(const char* path);
};

#endif
