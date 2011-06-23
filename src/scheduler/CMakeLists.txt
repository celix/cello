SET(SRC main.cpp)

ADD_EXECUTABLE(scheduler ${SRC})

TARGET_LINK_LIBRARIES(scheduler libglog.a)
TARGET_LINK_LIBRARIES(scheduler pthread)
