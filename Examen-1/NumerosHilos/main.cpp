#include <iostream>
#include <ctime>
#include <pthread.h>
#include "Numeros.h"
#include "Mutex.h"

using namespace std;

// Shared variables
Mutex * mutex;
std::vector<std::vector<int>> ready;
int range = 0;
int rangoMaster = 0;

/*
*
*/
void * hiloNumero( void * id )
{
    Numeros * num;
    mutex = new Mutex();
    num = new Numeros();
    //int identificacion = *((int*) id);
    long identificacion = (long) id;
    int rango;
    int inicio;
    int fin;

    std::vector<std::vector<int>> oneMore;
    std::vector<int> actual;

    if(identificacion == 0)
    {
        rango = rangoMaster;
    }
    else
    {
        rango = range;
    }

    inicio = (rango * identificacion) + 1;
    fin = rango * (identificacion+1);

    mutex->Lock();
    oneMore = num->Pares(inicio, fin);


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
    int cant_hilos = 0;
    std::vector<int> actual;

    //start = clock();

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
        range = limite/cant_hilos;
        rangoMaster = limite/cant_hilos;
    }
    else
    {
        range = limite/cant_hilos;
        rangoMaster = (limite - (range*cant_hilos));
    }

    Crear((long)cant_hilos);


    for(size_t num = 0; num < ready.size(); num++)
    {
        actual = ready[num];
        printf( "%d = %d + %d. \n",actual[0], actual[1], actual[2]);
    }

    finish = clock();
    time = finish - start;
    printf("Tiempo transcurrido: %f. \n", time);
}
