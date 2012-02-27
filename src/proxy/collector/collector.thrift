struct ExecutorStat {
    1: string fr_name,
    2: i64 task_id,
    3: double used_cpu,
    4: i32 used_memory,
    5: i32 task_num,
}

struct MachineInfo {
    1: string endpoint,
    2: double usage,
    3: i32 cpu,
    4: i32 memory,
    5: double avail_cpu,
    6: i32 avail_memory,
    7: list<ExecutorStat> executor_list,
}

service Collector {

    /// @brief: match machine for task
    string Match(1: string str_ad),

    /// @brief: heartbeat between collector and cellet
    void Heartbeat(1: MachineInfo info)
}
