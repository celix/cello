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

SET(CELLET_THRIFT_SOURCE
    ${THRIFT_SOURCE_PREFIX}/cellet/gen-cpp/cellet_constants.cpp
    ${THRIFT_SOURCE_PREFIX}/cellet/gen-cpp/Cellet.h
    ${THRIFT_SOURCE_PREFIX}/cellet/gen-cpp/cellet_types.h
    ${THRIFT_SOURCE_PREFIX}/cellet/gen-cpp/cellet_constants.h
    ${THRIFT_SOURCE_PREFIX}/cellet/gen-cpp/Cellet.cpp
    ${THRIFT_SOURCE_PREFIX}/cellet/gen-cpp/cellet_types.cpp)

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

ADD_CUSTOM_COMMAND (
    OUTPUT ${CELLET_THRIFT_SOURCE}
    COMMAND thrift --gen cpp -r -o ${CMAKE_SOURCE_DIR}/src/proxy/cellet
    ${CMAKE_SOURCE_DIR}/src/proxy/cellet/cellet.thrift
)

SET(COMMON_SOURCE
    ../common/cond.cpp
    ../common/policy.cpp
    ../common/register.cpp
    ../common/xml_handler.cpp
    ../common/thread.cpp)

SET(WRAPPER_SOURCE ../proxy/scheduler_wrapper.cpp)

SET(SRC
    ${SCHEDULER_THRIFT_SOURCE}
    ${COLLECTOR_THRIFT_SOURCE}
    ${CELLET_THRIFT_SOURCE}
    ${COMMON_SOURCE}
    ${WRAPPER_SOURCE}
    scheduler_service.cpp
    components_manager.cpp
    task.cpp
    state.cpp
    event.cpp
    task_queue.cpp
    framework.cpp
    autoscale_framework.cpp
    work_thread.cpp
    task_pool.cpp
    framework_pool.cpp
    autoscale_pool.cpp
    standard_pool.cpp
    handler.cpp
    dispatcher.cpp
    scheduler.cpp)

ADD_EXECUTABLE(scheduler ${SRC})

TARGET_LINK_LIBRARIES(scheduler glog)
TARGET_LINK_LIBRARIES(scheduler pthread)
TARGET_LINK_LIBRARIES(scheduler thrift)
TARGET_LINK_LIBRARIES(scheduler gflags)
TARGET_LINK_LIBRARIES(scheduler gflags)
TARGET_LINK_LIBRARIES(scheduler classad)
TARGET_LINK_LIBRARIES(scheduler xerces-c)

INSTALL(TARGETS scheduler RUNTIME DESTINATION bin)

