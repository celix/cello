SET(THIRD_PARTY "../third_party")
SET(SRC "../src")

INCLUDE_DIRECTORIES(${THIRD_PARTY}/glog/include
                    ${THIRD_PARTY}/gflags/include
                    ${THIRD_PARTY}/gtest/include
                    ${THIRD_PARTY}/classad/include
                    ${THIRD_PARTY}/thrift/include/thrift
                    ${THIRD_PARTY}/boost/include
                    ${THIRD_PARTY}/xerces/include
                    ${SRC}/)

LINK_DIRECTORIES(${THIRD_PARTY}/glog/lib
                 ${THIRD_PARTY}/gflags/lib
                 ${THIRD_PARTY}/gtest/lib
                 ${THIRD_PARTY}/classad/lib
                 ${THIRD_PARTY}/thrift/lib
                 ${THIRD_PARTY}/boost/lib
                 ${THIRD_PARTY}/xerces/lib)

ADD_SUBDIRECTORY(thrift-test)
ADD_SUBDIRECTORY(xml-test)
