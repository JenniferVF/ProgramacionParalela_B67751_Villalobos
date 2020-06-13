fileR: Mutex.o FileReader.o main.o
	g++ -g Mutex.o FileReader.o main.o -lpthread -o fileR

fileR.o: FileReader.cpp
	g++ -c -g Mutex.cpp FileReader.cpp main.cpp
