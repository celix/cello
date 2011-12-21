SET(WRAPPER_SOURCE ../proxy/scheduler_wrapper.cpp)

SET(SRC
    cello_add.cpp
    configuration.cpp
    add_context.cpp
    ${WRAPPER_SOURCE})

ADD_EXECUTABLE(cello-add ${SRC})


TARGET_LINK_LIBRARIES(cello-add pthread)
TARGET_LINK_LIBRARIES(cello-add thrift)
TARGET_LINK_LIBRARIES(cello-add gflags)

INSTALL(TARGETS cello-add RUNTIME DESTINATION bin)

