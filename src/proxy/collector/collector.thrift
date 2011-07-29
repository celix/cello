struct MachineInfo {
    1: string endpoint,
    2: double usage,
    3: i32 cpu,
    4: i32 memory,
    5: double avail_cpu,
    6: i32 avail_memory,
    7: i32 task_num
}

service Collector {

    /// @brief: match machine for task
    string Match(1: string str_ad),

    /// @brief: heartbeat between collector and cellet
    void Heartbeat(1: MachineInfo info)
}
