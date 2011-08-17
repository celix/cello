SET(CELLET_PROXY ../proxy/cellet/gen-cpp)
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

SET(COMMON_SOURCE
    ../common/message_queue.cpp)

SET(SRC
    ${CELLET_RPC_SOURCE}
    ${COLLECTOR_RPC_SOURCE}
    ${COMMON_SOURCE}
    system.cpp
    executor.cpp
    executor_pool.cpp
    message_manager.cpp
    resource_manager.cpp
    container.cpp
    container_pool.cpp
    work_thread.cpp
    main.cpp)

ADD_EXECUTABLE(cellet ${SRC})

TARGET_LINK_LIBRARIES(cellet glog)
TARGET_LINK_LIBRARIES(cellet pthread)
TARGET_LINK_LIBRARIES(cellet thrift)
TARGET_LINK_LIBRARIES(cellet gflags)
TARGET_LINK_LIBRARIES(cellet classad)

INSTALL(TARGETS cellet RUNTIME DESTINATION bin)

