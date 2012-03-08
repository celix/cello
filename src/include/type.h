#ifndef SRC_INCLUDE_TYPE_H
#define SRC_INCLUDE_TYPE_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <string>

using std::string;

static const int TIME_OUT = 2000;
static const key_t EXECUTOR_START_KEY = 7000;
static const key_t EXECUTOR_STATE_KEY = 7001;
static const key_t EXECUTOR_CONTROL_KEY = 7002;
static const key_t RESOURCE_INFO_KEY = 7003;

static const string DFS_PREFIX="/tmp/cello";

enum queue_type {
    WAIT_QUEUE,
    RUN_QUEUE
};

enum event_type {
    STATE_EVENT,
    ACTION_EVENT
};

enum ExecutorState {
    EXECUTOR_WAIT,
    EXECUTOR_RUN,
    EXECUTOR_FINISHED,
    EXECUTOR_KILLED
};

enum ContainerState {
    CONTAINER_INIT = 0,
    CONTAINER_STARTED,
    CONTAINER_FINISHED
};

#endif
