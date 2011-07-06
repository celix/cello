struct TaskInfo {
    1: required string framework_name,
    2: required string cmd,
    3: optional string arguments,
    4: optional string candidate_ips,         // not support now
    5: optional double cpu,                   // not support now
    6: optional double memory                 // not support now
}

service Scheduler {

    // for api
    i64 Submit(1: TaskInfo task_info),
    TaskInfo Query(1: i64 task_id),           // not support now
    i32 RemoveTask(1: i64 task_id),           // not support now
    
    // for cellet
    i32 TaskStarted(1: i64 task_id, 2: bool status),
    i32 TaskFinished(1: i64 task_id, 2: bool status)
}
