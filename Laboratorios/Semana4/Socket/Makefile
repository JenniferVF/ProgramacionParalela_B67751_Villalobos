MirrorServerTest: MirrorServerTest.o Socket.o
	g++ -g MirrorServerTest.o Socket.o -o MirrorServerTest

MirrorClientTest: MirrorClientTest.o Socket.o
	g++ -g MirrorClientTest.o Socket.o -o MirrorClientTest

Socket.o: Socket.h Socket.cpp
	g++ -g -c Socket.cpp

MirrorServerTest.o: MirrorServerTest.cpp Socket.h
	g++ -g -c MirrorServerTest.cpp
