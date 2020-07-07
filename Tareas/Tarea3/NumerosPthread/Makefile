num: Mutex.o main.o num.o 
	g++ -g Numeros.o Mutex.o main.o -lpthread -o num

num.o: Numeros.cpp
	g++ -c -g Numeros.cpp Mutex.cpp main.cpp -lpthread
	 
