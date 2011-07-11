SET(CELLET_PROXY ../proxy/scheduler/gen-cpp)
SET(CELLET_RPC_SOURCE
    ${CELLET_PROXY}/Scheduler.cpp
    ${CELLET_PROXY}/scheduler_types.cpp
    ${CELLET_PROXY}/scheduler_constants.cpp
    ${CELLET_PROXY}/Cellet.cpp
    ${CELLET_PROXY}/cellet_types.cpp
    ${CELLET_PROXY}/cellet_constants.cpp)

SET(COLLECTOR_PROXY ../proxy/collector/gen-cpp)
SET(COLLECTOR_RPC_SOURCE
    ${COLLECTOR_PROXY}/Collector.cpp
    ${COLLECTOR_PROXY}/collector_types.cpp
    ${COLLECTOR_PROXY}/collector_constants.cpp)

SET(SRC
    ${CELLET_RPC_SOURCE}
    ${COLLECTOR_RPC_SOURCE}
    system.cpp
    main.cpp)

ADD_EXECUTABLE(cellet ${SRC})

TARGET_LINK_LIBRARIES(cellet glog)
TARGET_LINK_LIBRARIES(cellet pthread)
TARGET_LINK_LIBRARIES(cellet thrift)
TARGET_LINK_LIBRARIES(cellet gflags)
TARGET_LINK_LIBRARIES(cellet gflags)
TARGET_LINK_LIBRARIES(cellet classad)
