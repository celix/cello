ADD_EXECUTABLE(read_xml read_xml.cpp)

TARGET_LINK_LIBRARIES(read_xml xerces-c)

INSTALL(TARGETS read_xml RUNTIME DESTINATION test/test)
INSTALL(FILES test.xml DESTINATION test/test)
