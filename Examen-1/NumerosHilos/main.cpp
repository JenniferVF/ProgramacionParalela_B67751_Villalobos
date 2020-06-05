#include <iostream>
#include <ctime>
#include <pthread.h>
#include "Numeros.h"
#include "Mutex.h"

using namespace std;

// Shared variables
Mutex * mutex;
std::vector<std::vector<int>> ready;

/*
*
*/
void * hiloNumero( void * rango )
{
    Numeros * num;
    mutex = new Mutex();
    num = new Numeros();
    int range = *((int*) rango);
    int id = 0;

    std::vector<std::vector<int>> oneMore;
    std::vector<int> actual;

    mutex->Lock();
    oneMore = num->Pares(range, id);


    for(size_t i = 0; i < oneMore.size(); i++)
    {
        actual = oneMore[i];
        ready.push_back(actual);
    }

    mutex->Unlock();
}


/*
*
*/
void Crear( long cantidad )
{
    long hilo;
    pthread_t * numeros;

    numeros = (pthread_t *) calloc( cantidad, sizeof( pthread_t ) );

    for ( hilo = 0; hilo < cantidad; hilo++ )
    {
        pthread_create( & numeros[ hilo ], NULL, hiloNumero, (void *) hilo );
    }


    for ( hilo = 0; hilo < cantidad; hilo++ )
    {
        pthread_join( numeros[ hilo ], NULL );
    }

    free( numeros );

}



int main(int  argc, char *argv[])
{
    Numeros * number;
    number = new Numeros();


    clock_t start, finish; //Variables para medir el tiempo.
    double time = 0; //Resultado del tiempo transcurrido
    int limite = 0;
    int rango = 0;
    int rangoMaster = 0;
    int cant_hilos = 0;

    start = clock();

    //Se le pide al usuario que ingrese un numero (el limite).
    if (argc != 3)
    {
        printf("Ingrese: %s <cantidad limite> y <numero de hilos>. \n", argv[0]);
        exit(1);
    }

    limite = atoi(argv[1]);
    cant_hilos = atoi(argv[2]);

    if(limite%cant_hilos == 0)
    {
        rango = limite/cant_hilos;
        rangoMaster = limite/cant_hilos;
    }
    else
    {
        rango = limite/cant_hilos;
        rangoMaster = (limite - (rango*cant_hilos));
    }



    finish = clock();
    time = finish - start;
    printf("Tiempo transcurrido: %f. \n", time);
}
