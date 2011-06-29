SET (TEST
     message_queue_test
     mutex_test
     rwlock_test
     get_ip_test
     block_queue_test
     singleton_test)

ADD_EXECUTABLE(message_queue_test message_queue_test.cpp message_queue.cpp)
ADD_EXECUTABLE(mutex_test mutex_test.cpp)
ADD_EXECUTABLE(rwlock_test rwlock_test.cpp)
ADD_EXECUTABLE(block_queue_test block_queue_test.cpp cond.cpp)
ADD_EXECUTABLE(get_ip_test get_ip_test.cpp)
ADD_EXECUTABLE(singleton_test singleton_test.cpp)

FOREACH(t ${TEST})
    TARGET_LINK_LIBRARIES(${t} libgtest.a)
    TARGET_LINK_LIBRARIES(${t} libglog.a)
    TARGET_LINK_LIBRARIES(${t} pthread)
ENDFOREACH(t ${TEST})

