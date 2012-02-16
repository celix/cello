# - Find Thrift executable, header and library
#
# The module defines the following variables:
#
# THRIFT_EXECUTABLE - path to the thrift program
# THRIFT_INCLUDE_PATH - path to the include file
# THRIFT_LIBRARY_PATH - path to the library file

FIND_PROGRAM(THRIFT_EXECUTABLE thrift DOC "path to the thrift program")
MARK_AS_ADVANCED(THRIFT_EXECUTABLE)

IF (THRIFT_EXECUTABLE)
    MESSAGE(STATUS "check thrift....... yes")
ELSE()
    MESSAGE(FATAL_ERROR "thrift executable not found")
ENDIF(THRIFT_EXECUTABLE)

FIND_PATH(THRIFT_INCLUDE_PATH thrift/config.h)

IF (THRIFT_INCLUDE_PATH)
    SET(THRIFT_INCLUDE_PATH "${THRIFT_INCLUDE_PATH}/thrift")
    MESSAGE(STATUS "thrift include directory: ${THRIFT_INCLUDE_PATH}")
ELSE()
    MESSAGE(FATAL_ERROR "thrift include directory not found")
ENDIF(THRIFT_INCLUDE_PATH)

FIND_LIBRARY(THRIFT_LIBRARY_PATH thrift)

IF (THRIFT_LIBRARY_PATH)
    MESSAGE(STATUS "thrift libraray directory: ${THRIFT_LIBRARY_PATH}")
ELSE()
    MESSAGE(FATAL_ERROR "thrift library not found")
ENDIF(THRIFT_LIBRARY_PATH)

