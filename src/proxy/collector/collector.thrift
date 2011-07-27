struct MachineInfo {
    1: required string endpoint,
    2: required double usage,
    3: optional i32 cpu,
    4: optional i32 memory,
    5: optional double avail_cpu,
    6: optional i32 avail_memory,
    7: optional i32 task_num
}

service Collector {

    /// @brief: match machine for task
    string Match(1: string str_ad),

    /// @brief: heartbeat between collector and cellet
    void Heartbeat(1: MachineInfo info)
}
