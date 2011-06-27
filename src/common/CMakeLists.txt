ADD_EXECUTABLE(message_queue_test message_queue_test.cpp message_queue.cpp)

TARGET_LINK_LIBRARIES(message_queue_test libgtest.a)
TARGET_LINK_LIBRARIES(message_queue_test libglog.a)
TARGET_LINK_LIBRARIES(message_queue_test pthread)

ADD_EXECUTABLE(mutex_test mutex_test.cpp)

TARGET_LINK_LIBRARIES(mutex_test libgtest.a)
TARGET_LINK_LIBRARIES(mutex_test libglog.a)
TARGET_LINK_LIBRARIES(mutex_test pthread)

ADD_EXECUTABLE(rwlock_test rwlock_test.cpp)

TARGET_LINK_LIBRARIES(rwlock_test libgtest.a)
TARGET_LINK_LIBRARIES(rwlock_test libglog.a)
TARGET_LINK_LIBRARIES(rwlock_test pthread)
