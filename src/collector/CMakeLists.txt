SET(COLLECTOR_PROXY ../proxy/collector/gen-cpp)
SET(COLLECTOR_RPC_SOURCE
    ${COLLECTOR_PROXY}/Collector.cpp
    ${COLLECTOR_PROXY}/collector_types.cpp
    ${COLLECTOR_PROXY}/collector_constants.cpp)
SET(COMMON_SOURCE
    ../common/block_queue.cpp
    ../common/cond.cpp
    ../common/thread.cpp)

SET(SRC
    ${COLLECTOR_RPC_SOURCE}
    ${COMMON_SOURCE}
    machine_pool.cpp
    machine.cpp
    monitor.cpp
    framework.cpp
    trigger.cpp
    main.cpp)

SET(MOCK
    ${COLLECTOR_RPC_SOURCE}
    cellet_mock.cpp)

ADD_EXECUTABLE(collector ${SRC})
ADD_EXECUTABLE(cellet_mock ${MOCK})

TARGET_LINK_LIBRARIES(collector glog)
TARGET_LINK_LIBRARIES(collector pthread)
TARGET_LINK_LIBRARIES(collector thrift)
TARGET_LINK_LIBRARIES(collector gflags)
TARGET_LINK_LIBRARIES(collector gflags)
TARGET_LINK_LIBRARIES(collector classad)

TARGET_LINK_LIBRARIES(cellet_mock thrift)
TARGET_LINK_LIBRARIES(cellet_mock pthread)
TARGET_LINK_LIBRARIES(cellet_mock glog)

INSTALL(TARGETS collector RUNTIME DESTINATION bin)

