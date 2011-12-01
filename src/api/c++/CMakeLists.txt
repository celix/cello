SET(SCHEDULER_PROXY ../proxy/scheduler/gen-cpp)
SET(SCHEDULER_RPC_SOURCE
    ${SCHEDULER_PROXY}/Scheduler.cpp
    ${SCHEDULER_PROXY}/scheduler_types.cpp
    ${SCHEDULER_PROXY}/scheduler_constants.cpp)

SET(COLLECTOR_PROXY ../proxy/collector/gen-cpp)
SET(COLLECTOR_RPC_SOURCE
    ${COLLECTOR_PROXY}/Collector.cpp
    ${COLLECTOR_PROXY}/collector_types.cpp
    ${COLLECTOR_PROXY}/collector_constants.cpp)

SET(CELLET_PROXY ../proxy/cellet/gen-cpp)
SET(CELLET_RPC_SOURCE
    ${CELLET_PROXY}/Cellet.cpp
    ${CELLET_PROXY}/cellet_types.cpp
    ${CELLET_PROXY}/cellet_constants.cpp)

SET(COMMON_SOURCE
    ../common/cond.cpp
    ../common/block_queue.cpp)

SET(SRC
    ${SCHEDULER_RPC_SOURCE}
    ${COLLECTOR_RPC_SOURCE}
    ${CELLET_RPC_SOURCE}
    ${COMMON_SOURCE}
    task.cpp
    state.cpp
    event.cpp
    task_queue.cpp
    framework.cpp
    work_thread.cpp
    task_pool.cpp
    framework_pool.cpp
    main.cpp)

ADD_EXECUTABLE(scheduler ${SRC})

TARGET_LINK_LIBRARIES(scheduler glog)
TARGET_LINK_LIBRARIES(scheduler pthread)
TARGET_LINK_LIBRARIES(scheduler thrift)
TARGET_LINK_LIBRARIES(scheduler gflags)
TARGET_LINK_LIBRARIES(scheduler gflags)
TARGET_LINK_LIBRARIES(scheduler classad)
TARGET_LINK_LIBRARIES(scheduler xerces-c)

INSTALL(TARGETS scheduler RUNTIME DESTINATION bin)

