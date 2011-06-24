SET(THIRD_PARTY "../third_party")

INCLUDE_DIRECTORIES(${THIRD_PARTY}/glog/include
                    ${THIRD_PARTY}/gflags/include
                    ${THIRD_PARTY}/gtest/include
                    ${THIRD_PARTY}/classad/include)

LINK_DIRECTORIES(${THIRD_PARTY}/glog/lib
                 ${THIRD_PARTY}/gflags/lib
                 ${THIRD_PARTY}/gtest/lib
                 ${THIRD_PARTY}/classad/lib)

ADD_SUBDIRECTORY(scheduler)
ADD_SUBDIRECTORY(cellet)
ADD_SUBDIRECTORY(collector)
ADD_SUBDIRECTORY(include)
