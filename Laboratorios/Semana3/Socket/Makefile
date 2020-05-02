ipv4-test: ipv4-test.o Socket.o
	g++ -g ipv4-test.o Socket.o -o ipv4-test

Socket.o: Socket.h Socket.cpp
	g++ -g -c Socket.cpp

ipv4-test.o: ipv4-test.cpp Socket.h
	g++ -g -c ipv4-test.cpp

ipv6-test: ipv6-test.o Socket.o
	g++ -g ipv6-test.o Socket.o -o ipv6-test

ipv6-test.o: ipv6-test.cpp Socket.h
	g++ -g -c ipv6-test.cpp
