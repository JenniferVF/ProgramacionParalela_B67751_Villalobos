#include <iostream>
#include <chrono>
#include "Numeros.h"

using namespace std;




int main(int  argc, char *argv[])
{
    Numeros * number;
    number = new Numeros();
    double tiempo;

    auto start = chrono::high_resolution_clock::now();


    //Se le pide al usuario que ingrese un numero (el limite).
    if (argc != 2)
    {
        printf("Ingrese: %s <cantidad limite>. \n", argv[0]);
        exit(1);
    }

    number->Pares(atoi(argv[1]));

    auto finish = chrono::high_resolution_clock::now();

    tiempo = chrono::duration<double, milli>(finish-start).count();
    printf("Tiempo transcurrido en milisegundos: %f. \n", tiempo);
}
