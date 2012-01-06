SET(THIRD_PARTY "../third_party")
SET(SRC ".")

INCLUDE_DIRECTORIES(
    /usr/local/include/thrift/
    /usr/local/jdk1.6.0_03/include/
    /usr/local/jdk1.6.0_03/include/linux/
    ${SRC}/)

LINK_DIRECTORIES(
    /usr/local/jdk1.6.0_03/jre/lib/i386/client
    ${THIRD_PARTY}/lxc/lib)

ADD_SUBDIRECTORY(scheduler)
ADD_SUBDIRECTORY(cellet)
ADD_SUBDIRECTORY(collector)
ADD_SUBDIRECTORY(common)
ADD_SUBDIRECTORY(test)
ADD_SUBDIRECTORY(tools)
