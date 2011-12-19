SET(THIRD_PARTY "../third_party")
SET(SRC ".")

INCLUDE_DIRECTORIES(
    /usr/local/include/thrift/
    ${SRC}/)

LINK_DIRECTORIES(
    ${THIRD_PARTY}/lxc/lib)

ADD_SUBDIRECTORY(scheduler)
ADD_SUBDIRECTORY(cellet)
ADD_SUBDIRECTORY(collector)
ADD_SUBDIRECTORY(common)
#ADD_SUBDIRECTORY(test)
#ADD_SUBDIRECTORY(tools)
