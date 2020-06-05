#include <iostream>
#include <ctime>
//#include <pthread.h>
#include "Numeros.h"

using namespace std;




int main(int  argc, char *argv[])
{
    Numeros * number;
    number = new Numeros();

    clock_t start, finish; //Variables para medir el tiempo.
    double time = 0; //Resultado del tiempo transcurrido


    start = clock();

    //Se le pide al usuario que ingrese un numero (el limite).
    if (argc != 2)
    {
        printf("Ingrese: %s <cantidad limite>. \n", argv[0]);
        exit(1);
    }

    number->Pares(atoi(argv[1]));

    finish = clock();
    time = finish - start;
    printf("Tiempo transcurrido: %f. \n", time);
}
