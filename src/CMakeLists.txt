SET(THIRD_PARTY "../third_party")
SET(SRC ".")

INCLUDE_DIRECTORIES(${THIRD_PARTY}/glog/include
                    ${THIRD_PARTY}/gflags/include
                    ${THIRD_PARTY}/gtest/include
                    ${THIRD_PARTY}/classad/include
                    ${THIRD_PARTY}/thrift/include/thrift
                    ${SRC}/)

LINK_DIRECTORIES(${THIRD_PARTY}/glog/lib
                 ${THIRD_PARTY}/gflags/lib
                 ${THIRD_PARTY}/gtest/lib
                 ${THIRD_PARTY}/thrift/lib
                 ${THIRD_PARTY}/classad/lib)

ADD_SUBDIRECTORY(cellet)
ADD_SUBDIRECTORY(collector)
ADD_SUBDIRECTORY(common)
ADD_SUBDIRECTORY(scheduler)
