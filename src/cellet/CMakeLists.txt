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
    ../common/message_queue.cpp
    ../common/hdfs_filesystem.cpp
    ../common/register.cpp
    ../common/policy.cpp
    ../common/xml_handler.cpp
    ../proxy/collector_wrapper.cpp)

SET(SRC
    ${SCHEDULER_THRIFT_SOURCE}
    ${COLLECTOR_THRIFT_SOURCE}
    ${CELLET_THRIFT_SOURCE}
    ${COMMON_SOURCE}
    config.cpp
    system.cpp
    executor.cpp
    executor_pool.cpp
    message_manager.cpp
    resource_manager.cpp
    container.cpp
    container_pool.cpp
    work_thread.cpp
    event.cpp
    cellet_service.cpp
    cellet.cpp)

ADD_EXECUTABLE(cellet ${SRC})
ADD_EXECUTABLE(system_test system.cpp system_test.cpp)
ADD_EXECUTABLE(container_test container.cpp container_test.cpp system.cpp
                              config.cpp message_manager.cpp ${COMMON_SOURCE})

TARGET_LINK_LIBRARIES(cellet glog)
TARGET_LINK_LIBRARIES(cellet pthread)
TARGET_LINK_LIBRARIES(cellet thrift)
TARGET_LINK_LIBRARIES(cellet gflags)
TARGET_LINK_LIBRARIES(cellet classad)
TARGET_LINK_LIBRARIES(cellet lxc)
TARGET_LINK_LIBRARIES(cellet hdfs)
TARGET_LINK_LIBRARIES(cellet jvm)
TARGET_LINK_LIBRARIES(cellet xerces-c)

TARGET_LINK_LIBRARIES(system_test gtest pthread)

TARGET_LINK_LIBRARIES(container_test gtest pthread glog lxc gflags hdfs xerces-c jvm)

INSTALL(TARGETS cellet RUNTIME DESTINATION bin)
INSTALL(TARGETS system_test RUNTIME DESTINATION test/cellet)
