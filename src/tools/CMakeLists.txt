SET(WRAPPER_SOURCE ../proxy/scheduler_wrapper.cpp)

SET(CELLO_ADD
    cello_add.cpp
    configuration.cpp
    framework_configuration.cpp
    add_context.cpp
    ${WRAPPER_SOURCE})

ADD_EXECUTABLE(cello-add ${CELLO_ADD})


TARGET_LINK_LIBRARIES(cello-add pthread)
TARGET_LINK_LIBRARIES(cello-add thrift)
TARGET_LINK_LIBRARIES(cello-add gflags)

INSTALL(TARGETS cello-add RUNTIME DESTINATION bin)

SET(CELLO_SUBMIT
    cello_submit.cpp
    configuration.cpp
    task_configuration.cpp
    submit_context.cpp
    ${WRAPPER_SOURCE})

ADD_EXECUTABLE(cello-submit ${CELLO_SUBMIT})


TARGET_LINK_LIBRARIES(cello-submit pthread)
TARGET_LINK_LIBRARIES(cello-submit thrift)
TARGET_LINK_LIBRARIES(cello-submit gflags)

INSTALL(TARGETS cello-submit RUNTIME DESTINATION bin)

