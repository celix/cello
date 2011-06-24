SET(SRC get_ip_test.cpp)

ADD_EXECUTABLE(get_ip_test ${SRC})

TARGET_LINK_LIBRARIES(get_ip_test libgtest.a)
TARGET_LINK_LIBRARIES(get_ip_test libglog.a)
TARGET_LINK_LIBRARIES(get_ip_test pthread)
