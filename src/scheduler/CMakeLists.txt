SET(SCHEDULER_PROXY ../proxy/scheduler/gen-cpp)
SET(SCHEDULER_RPC_SOURCE
    ${SCHEDULER_PROXY}/Scheduler.cpp
    ${SCHEDULER_PROXY}/scheduler_types.cpp
    ${SCHEDULER_PROXY}/scheduler_constants.cpp)

SET(SRC
    ${SCHEDULER_RPC_SOURCE}
    task.cpp
    framework.cpp
    work_thread.cpp
    task_pool.cpp
    main.cpp)

ADD_EXECUTABLE(scheduler ${SRC})

TARGET_LINK_LIBRARIES(scheduler libglog.a)
TARGET_LINK_LIBRARIES(scheduler pthread)
TARGET_LINK_LIBRARIES(scheduler thrift)
TARGET_LINK_LIBRARIES(scheduler libgflags.a)
