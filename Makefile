CC=g++
CFLAGS=-O2 -Wall
BUILD_FLAGS=-DNET_DEBUG
LDFLAGS=-dynamic
INCLUDES=-I. `pkg-config --cflags libevent`
LIBS=-L. `pkg-config --libs libevent`

all: exception.o socket.o connection.o src/server.hpp net.o
	${CC} ${LDFLAGS} ${LIBS} ${BUILD_FLAGS} ${INCLUDES} -levent \
	       exception.o socket.o connection.o net.o \
	       -o net-cpp-test src/main.cpp

net.o: src/net.cpp src/net.h
	${CC} -c ${INCLUDES} ${CFLAGS} ${BUILD_FLAGS} src/net.cpp

connection.o: src/connection.cpp src/connection.h
	${CC} -c ${INCLUDES} ${CFLAGS} ${BUILD_FLAGS} src/connection.cpp

socket.o: src/socket.cpp src/socket.h
	${CC} -c ${INCLUDES} ${CFLAGS} ${BUILD_FLAGS} src/socket.cpp

exception.o: src/exception.cpp src/exception.h
	${CC} -c ${INCLUDES} ${CFLAGS} ${BUILD_FLAGS} src/exception.cpp 

clean:
	rm -r *.o *.gch net-cpp-test

