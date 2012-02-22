SET(THRIFT_SOURCE_PREFIX ${CMAKE_SOURCE_DIR}/src/proxy)

SET(SCHEDULER_THRIFT_SOURCE
    ${THRIFT_SOURCE_PREFIX}/scheduler/gen-cpp/scheduler_constants.cpp
    ${THRIFT_SOURCE_PREFIX}/scheduler/gen-cpp/Scheduler.h
    ${THRIFT_SOURCE_PREFIX}/scheduler/gen-cpp/scheduler_types.h
    ${THRIFT_SOURCE_PREFIX}/scheduler/gen-cpp/scheduler_constants.h
    ${THRIFT_SOURCE_PREFIX}/scheduler/gen-cpp/Scheduler.cpp
    ${THRIFT_SOURCE_PREFIX}/scheduler/gen-cpp/scheduler_types.cpp)

ADD_CUSTOM_COMMAND (
    OUTPUT ${SCHEDULER_THRIFT_SOURCE}
    COMMAND thrift --gen cpp -r -o ${CMAKE_SOURCE_DIR}/src/proxy/scheduler
    ${CMAKE_SOURCE_DIR}/src/proxy/scheduler/scheduler.thrift
)

SET(WRAPPER_SOURCE ../proxy/scheduler_wrapper.cpp)
SET(COMMON_SOURCE
    ../common/hdfs_filesystem.cpp
    ../common/register.cpp
    ../common/policy.cpp
    ../common/xml_handler.cpp)

SET(CELLO_ADD
    cello_add.cpp
    configuration.cpp
    framework_configuration.cpp
    add_context.cpp
    ../common/xml_handler.cpp
    ${WRAPPER_SOURCE}
    ${SCHEDULER_THRIFT_SOURCE})

ADD_EXECUTABLE(cello-add ${CELLO_ADD})


TARGET_LINK_LIBRARIES(cello-add pthread)
TARGET_LINK_LIBRARIES(cello-add thrift)
TARGET_LINK_LIBRARIES(cello-add gflags)
TARGET_LINK_LIBRARIES(cello-add glog)
TARGET_LINK_LIBRARIES(cello-add xerces-c)

INSTALL(TARGETS cello-add RUNTIME DESTINATION bin)

SET(CELLO_SUBMIT
    cello_submit.cpp
    configuration.cpp
    task_configuration.cpp
    submit_context.cpp
    ${COMMON_SOURCE}
    ${WRAPPER_SOURCE}
    ${SCHEDULER_THRIFT_SOURCE})

ADD_EXECUTABLE(cello-submit ${CELLO_SUBMIT})


TARGET_LINK_LIBRARIES(cello-submit pthread)
TARGET_LINK_LIBRARIES(cello-submit thrift)
TARGET_LINK_LIBRARIES(cello-submit gflags)
TARGET_LINK_LIBRARIES(cello-submit glog)
TARGET_LINK_LIBRARIES(cello-submit xerces-c)
TARGET_LINK_LIBRARIES(cello-submit hdfs)

INSTALL(TARGETS cello-submit RUNTIME DESTINATION bin)

