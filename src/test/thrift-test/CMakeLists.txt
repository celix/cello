SET(CLIENT
    client.cpp
    gen-cpp/Echo.cpp
    gen-cpp/echo_constants.cpp
    gen-cpp/echo_types.cpp)
SET(SERVER
    server.cpp
    gen-cpp/Echo.cpp
    gen-cpp/echo_constants.cpp
    gen-cpp/echo_types.cpp)

ADD_EXECUTABLE(client ${CLIENT})
ADD_EXECUTABLE(server ${SERVER})

TARGET_LINK_LIBRARIES(client glog)
TARGET_LINK_LIBRARIES(client thrift)
TARGET_LINK_LIBRARIES(server thrift)
TARGET_LINK_LIBRARIES(server glog)

INSTALL(TARGETS client server RUNTIME DESTINATION test/test)
