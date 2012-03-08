include "../scheduler/scheduler.thrift"

service Cellet {
    bool StartTask(1: scheduler.TaskInfo info),
    i32 KillTask(1: i64 task_id)
}
