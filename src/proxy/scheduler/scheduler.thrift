struct TaskInfo {
    1: string framework_name,
    2: i64 id,
    3: string cmd,
    4: string arguments,
    5: string candidate_ips, 
    6: string transfer_files,
    7: double need_cpu,
    8: i32 need_memory
}

struct FrameworkInfo {
    1: string name,
    2: i32 quota,
    3: string command,
    4: string arguments,
    5: double cpu,
    6: i32 memory
}

service Scheduler {
    // for api
    i64 Submit(1: TaskInfo task_info),
    TaskInfo Query(1: i64 task_id),           // not support now
    i32 RemoveTask(1: i64 task_id),           // not support now
    
    // for cellet
    i32 TaskStarted(1: i64 task_id, 2: bool status),
    i32 TaskFinished(1: i64 task_id, 2: bool status),
    i32 AddFramework(1: FrameworkInfo framework_info),
    bool DeleteFramework(1: string name),
    i32 AddExecutor(1: string name),
    i32 DeleteExecutor(1: i64 executor_id)
}
