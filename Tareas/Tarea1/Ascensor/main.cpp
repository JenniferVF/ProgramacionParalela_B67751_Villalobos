/*
 * Esqueleto para el problema del ascensor utilizando PThreads
 * Tenemos el metodo principal encargado de la generacion de los hilos
 * Arrancamos con un hilo para simular el comportamiento del ascensor
 * y oleadas de personas que lo utilizan
 *
 * Author: Programacion Concurrente (Francisco Arroyo)
 * Version: 2020/Abr/23
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include "Ascensor.h"
#include "Persona.h"
#include "Mutex.h"


// Shared variables
Mutex * mutex;
std::vector<std::vector<int>> cola;

/*
 *  Rutina que ejecutan los hilos ascensor
 *
 */
void * hiloAscensor( void * param )
{
    Ascensor * ascensor;
    long identificacion;
    char  * rotulo;

    mutex = new Mutex();

    identificacion = (long) param;
    rotulo = (char *) calloc( 64, 1 );

    ascensor = new Ascensor( identificacion );

    mutex->Lock();
    ascensor->Display( rotulo );
    printf( "%s", rotulo );
    free( rotulo );
    mutex->Unlock();


    //while(ascensor->getCapacidad() == 0)
    //{
        rotulo = (char *) calloc( 64, 1 );
        mutex->Lock();
        ascensor->solicitud(cola.front(), rotulo);
        cola.pop_back();
        printf( "%s", rotulo );
        free( rotulo );
        mutex->Unlock();
    //}

    return NULL;

}


/*
 *  Rutina que crea los ascensores del edificio, en el caso de esta tarea solo 1
 *  Cada ascensor es representado por un hilo
 */
void * Ascensores( void * cantidad ) //Era un long
{
    long hilo;
    pthread_t * ascensor;

    ascensor = (pthread_t *) calloc( (long)cantidad, sizeof( pthread_t ) );

    for ( hilo = 0; hilo < (long)cantidad; hilo++ )
    {
        pthread_create( & ascensor[ hilo ], NULL, hiloAscensor, (void *) hilo );
    }

//    for ( hilo = 0; hilo < (long)cantidad; hilo++ )
//    {
//        pthread_join( ascensor[ hilo ], NULL );
//    }
//
//    free( ascensor );

}


void * hiloPersona( void * param)
{
    Persona * persona;
    char  * rotulo;
    long identificacion;

    mutex = new Mutex();

    identificacion = (long) param;
    persona = new Persona( identificacion );
    rotulo = (char *) calloc( 64, 1 );
    mutex->Lock();
    persona->Display( rotulo );
    printf( "%s", rotulo );
    free( rotulo );
    mutex->Unlock();

    rotulo = (char *) calloc( 64, 1 );
    mutex->Lock();
    cola.push_back(persona->llamado( rotulo ));
    printf( "%s", rotulo );
    free( rotulo );
    mutex->Unlock();

    return NULL;

}


/*
 *  Rutina que simula las oleadas de personas para uso del ascensor
 *  Las personas son generadas como hilos
 *
 */
void Personas( long cantidad )
{
    long hilo;
    pthread_t * personas;

    personas = (pthread_t *) calloc( cantidad, sizeof( pthread_t ) );

    for ( hilo = 0; hilo < cantidad; hilo++ )
    {
        pthread_create( & personas[ hilo ], NULL, hiloPersona, (void *) hilo );
    }


    for ( hilo = 0; hilo < cantidad; hilo++ )
    {
        pthread_join( personas[ hilo ], NULL );
    }

    free( personas );

}


/*
 *  Rutina principal para la creacion de los ascensores y las personas
 *  Restricciones de este proyecto programado
 *     - Solo un ascensor
 *
 */
int main( int argc, char ** argv )
{
    long hilos;
    clock_t start, finish;
    double used;
    pthread_t ascensor;
    long cantidad = 1;

    std::vector<int> prueba;
    prueba.push_back(1);
    prueba.push_back(10);
    prueba.push_back(4);
    cola.push_back(prueba);

    //srand( time( NULL ) );	// Puede poner esta linea en comentarios para generar una misma secuencia de numeros
    //Ascensores( 1 );
    printf("Ascensor[ 0 ]: No hay solicitudes pendientes.\n");
    pthread_create( & ascensor, NULL, Ascensores, (void *) cantidad );
    //usleep(50);

    Personas( 15 );	// Primera oleada

    sleep( 2 );


    //Personas( 10 );	// Segunda oleada


    pthread_join( ascensor, NULL );

}
