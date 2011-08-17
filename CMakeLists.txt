CMAKE_MINIMUM_REQUIRED(VERSION 2.8.1)

PROJECT(CELLO)

SET(CMAKE_CXX_FLAGS "-g -Wall")
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

SET(LIBRARY
    ${CMAKE_INSTALL_PREFIX}/third_party/boost/lib
    ${CMAKE_INSTALL_PREFIX}/third_party/glog/lib
    ${CMAKE_INSTALL_PREFIX}/third_party/gtest/lib
    ${CMAKE_INSTALL_PREFIX}/third_party/gflags/lib
    ${CMAKE_INSTALL_PREFIX}/third_party/classad/lib
    ${CMAKE_INSTALL_PREFIX}/third_party/xerces/lib
    ${CMAKE_INSTALL_PREFIX}/third_party/thrift/lib)

# the RPATH to be used when installing, but only if it's not a system directory
FOREACH(LIB ${LIBRARY})
    LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES
        "${LIB}" isSystemDir)
    IF("${isSystemDir}" STREQUAL "-1")
        LIST(APPEND CMAKE_INSTALL_RPATH ${LIB})
    ENDIF("${isSystemDir}" STREQUAL "-1")
ENDFOREACH(LIB ${LIBRARY})

MESSAGE(STATUS "Add Shared Library:")
FOREACH(msg ${CMAKE_INSTALL_RPATH})
    MESSAGE(STATUS ${msg})
ENDFOREACH(msg ${CMAKE_INSTALL_RPATH})


ADD_SUBDIRECTORY(src)

INSTALL(DIRECTORY log DESTINATION .)
