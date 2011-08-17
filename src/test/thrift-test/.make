BOOST_DIR = /usr/include/boost
THRIFT_DIR = /usr/include/thrift
LIB_DIR = /usr/lib

default: server client

GEN_SRC = ./gen-cpp/Echo.cpp ./gen-cpp/echo_types.cpp

client: client.cpp
	g++ -o client -I${THRIFT_DIR} -I${BOOST_DIR} -I./gen-cpp -L${LIB_DIR} -lthrift client.cpp ${GEN_SRC}

server: server.cpp
	g++ -o server -I${THRIFT_DIR} -I${BOOST_DIR} -I./gen-cpp -L${LIB_DIR} -lthrift server.cpp ${GEN_SRC}
