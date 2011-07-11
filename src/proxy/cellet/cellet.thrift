include "../scheduler/scheduler.thrift"

service Cellet {
    bool StartTask(1: scheduler.TaskInfo info)
}
