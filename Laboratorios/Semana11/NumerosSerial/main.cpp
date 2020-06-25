#include <iostream>
#include <ctime>
#include "Numeros.h"
#include "chrono.h"

using namespace std;




int main(int  argc, char *argv[])
{
    Numeros * number;
    number = new Numeros();

    Chrono inicio, fin;

    inicio.readTime();


    //Se le pide al usuario que ingrese un numero (el limite).
    if (argc != 2)
    {
        printf("Ingrese: %s <cantidad limite>. \n", argv[0]);
        exit(1);
    }

    number->Pares(atoi(argv[1]));

    fin.readTime();

    fin -= inicio;
    printf("Tiempo transcurrido en segundos: %d. \n", fin.getnSecs());
}
