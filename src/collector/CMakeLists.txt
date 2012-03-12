SET(THRIFT_SOURCE_PREFIX ${CMAKE_SOURCE_DIR}/src/proxy)
SET(COLLECTOR_THRIFT_SOURCE
    ${THRIFT_SOURCE_PREFIX}/collector/gen-cpp/collector_constants.cpp
    ${THRIFT_SOURCE_PREFIX}/collector/gen-cpp/Collector.h
    ${THRIFT_SOURCE_PREFIX}/collector/gen-cpp/collector_types.h
    ${THRIFT_SOURCE_PREFIX}/collector/gen-cpp/collector_constants.h
    ${THRIFT_SOURCE_PREFIX}/collector/gen-cpp/Collector.cpp
    ${THRIFT_SOURCE_PREFIX}/collector/gen-cpp/collector_types.cpp)

SET(SCHEDULER_THRIFT_SOURCE
    ${THRIFT_SOURCE_PREFIX}/scheduler/gen-cpp/scheduler_constants.cpp
    ${THRIFT_SOURCE_PREFIX}/scheduler/gen-cpp/Scheduler.h
    ${THRIFT_SOURCE_PREFIX}/scheduler/gen-cpp/scheduler_types.h
    ${THRIFT_SOURCE_PREFIX}/scheduler/gen-cpp/scheduler_constants.h
    ${THRIFT_SOURCE_PREFIX}/scheduler/gen-cpp/Scheduler.cpp
    ${THRIFT_SOURCE_PREFIX}/scheduler/gen-cpp/scheduler_types.cpp)

ADD_CUSTOM_COMMAND (
    OUTPUT ${COLLECTOR_THRIFT_SOURCE}
    COMMAND thrift --gen cpp -r -o ${CMAKE_SOURCE_DIR}/src/proxy/collector
    ${CMAKE_SOURCE_DIR}/src/proxy/collector/collector.thrift
)

ADD_CUSTOM_COMMAND (
    OUTPUT ${SCHEDULER_THRIFT_SOURCE}
    COMMAND thrift --gen cpp -r -o ${CMAKE_SOURCE_DIR}/src/proxy/scheduler
    ${CMAKE_SOURCE_DIR}/src/proxy/scheduler/scheduler.thrift
)

SET(COMMON_SOURCE
    ../common/cond.cpp
    ../common/thread.cpp
    ../common/policy.cpp
    ../common/xml_handler.cpp)

SET(SRC
    ${COLLECTOR_THRIFT_SOURCE}
    ${SCHEDULER_THRIFT_SOURCE}
    ${COMMON_SOURCE}
    ../proxy/collector_wrapper.cpp
    machine_pool.cpp
    machine.cpp
    monitor.cpp
    framework.cpp
    trigger.cpp
    trigger_queue.cpp
    collector_service.cpp
    framework_in_machine.cpp
    executor_in_machine.cpp
    collector.cpp)

SET(MOCK
    ${COLLECTOR_THRIFT_SOURCE}
    cellet_mock.cpp)

ADD_EXECUTABLE(collector ${SRC})
ADD_EXECUTABLE(cellet_mock ${MOCK})

TARGET_LINK_LIBRARIES(collector glog)
TARGET_LINK_LIBRARIES(collector pthread)
TARGET_LINK_LIBRARIES(collector thrift)
TARGET_LINK_LIBRARIES(collector gflags)
TARGET_LINK_LIBRARIES(collector gflags)
TARGET_LINK_LIBRARIES(collector classad)
TARGET_LINK_LIBRARIES(collector xerces-c)

TARGET_LINK_LIBRARIES(cellet_mock thrift)
TARGET_LINK_LIBRARIES(cellet_mock pthread)
TARGET_LINK_LIBRARIES(cellet_mock glog)

INSTALL(TARGETS collector RUNTIME DESTINATION bin)

