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

SET(SRC
    ${COLLECTOR_RPC_SOURCE}
    main.cpp)

ADD_EXECUTABLE(cellet ${SRC})

TARGET_LINK_LIBRARIES(cellet glog)
TARGET_LINK_LIBRARIES(cellet pthread)
TARGET_LINK_LIBRARIES(cellet thrift)
TARGET_LINK_LIBRARIES(cellet gflags)
TARGET_LINK_LIBRARIES(cellet gflags)
TARGET_LINK_LIBRARIES(cellet classad)
