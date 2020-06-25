#include <iostream>
#include <ctime>
#include <pthread.h>
#include <math.h>
#include "Numeros.h"
#include "Mutex.h"
#include "Chrono.h"

using namespace std;


// Shared variables
Mutex * mutex;
vector<vector<int>> ready; //Vector que almacena todas las sumas de todos los numeros.
int range = 0;  //Almacena el rango de cada hilo
int top = 0;  //Almacena el limite ingresado por el usuario



/*
*Metodo encargado de realizar la
*rutina de cada hiloNumero
*/
void * hiloNumero( void * id )
{
    Numeros * num;
    mutex = new Mutex();
    num = new Numeros();
    long identificacion = (long) id;
    int inicio;
    int fin;

    vector<vector<int>> oneMore; //Vector que almacena todas las sumas realizadas en el rango correspondiente.
    vector<int> actual; //Vector que almacena solo una suma y la agrega al vector global.

    //Se crea el rango de cada hilo segun su id.
    inicio = (range * identificacion) + 1;
    fin = range * (identificacion+1);

    //Si el final del rango es mayor que el limite ingresado por el usuario.
    if (fin > top)
    {
        fin = top;
    }

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
*Metodo encargado de crear cierta
*cantidad de hilosNumero, segun el
*parametro ingresado.
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

    //Se espera por la finalizacion de todos los hilos creados.
    for ( hilo = 0; hilo < cantidad; hilo++ )
    {
        pthread_join( numeros[ hilo ], NULL );
    }

    free( numeros );

}



int main(int  argc, char *argv[])
{
    Chrono inicio, fin;
    int limite = 0;
    int cant_hilos = 0;
    int rango = 0;
    std::vector<int> actual;

    inicio.readTime();

    //Se le pide al usuario que ingrese un numero (el limite).
    if (argc != 3)
    {
        printf("Ingrese: %s <cantidad limite> y <numero de hilos>. \n", argv[0]);
        exit(1);
    }

    limite = atoi(argv[1]);
    cant_hilos = atoi(argv[2]);
    top = limite;

    //En caso de que la cantidad de numeros no sea divisible entre la cantidad de hilos.
    rango = floor(limite/cant_hilos);
    range = (limite - rango*(cant_hilos-1));

    //Se crean los hilosNumeros
    Crear((long)cant_hilos);


    //Imprime el resultado
    for(size_t num = 0; num < ready.size(); num++)
    {
        actual = ready[num];
        printf( "%d = %d + %d. \n",actual[0], actual[1], actual[2]);
    }

   fin.readTime();

   fin -= inicio;
   printf("Tiempo transcurrido en segundos: %d. \n", fin.getnSecs());
}
