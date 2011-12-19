struct TaskInfo {
    1: string framework_name,
    2: i64 id,
    3: string cmd,
    4: string arguments,
    5: string candidate_ips, 
    6: double need_cpu,
    7: i32 need_memory
}

service Scheduler {

    // for api
    i64 Submit(1: TaskInfo task_info),
    TaskInfo Query(1: i64 task_id),           // not support now
    i32 RemoveTask(1: i64 task_id),           // not support now
    
    // for cellet
    i32 TaskStarted(1: i64 task_id, 2: bool status),
    i32 TaskFinished(1: i64 task_id, 2: bool status),
}
