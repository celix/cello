#ifndef SRC_INCLUDE_TYPE_H
#define SRC_INCLUDE_TYPE_H

#include <sys/types.h>
#include <sys/ipc.h>

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

enum ExecutorState {
    EXECUTOR_WAIT,
    EXECUTOR_RUN,
    EXECUTOR_FINISHED
};

enum ContainerState {
    CONTAINER_INIT = 0,
    CONTAINER_STARTED,
    CONTAINER_FINISHED
};

#endif
