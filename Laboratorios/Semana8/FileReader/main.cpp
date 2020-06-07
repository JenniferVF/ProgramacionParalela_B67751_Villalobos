#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "FileReader.h"
#include "Mutex.h"

using namespace std;

//Shared variables
//std::map<std::string, int> etiquetas;
Mutex * mutex;
bool busy = false;

/*
*Estructura que almacena los datos necesitados
*por los hilos trabajadores.
*/
struct Data_Worker
{
    int identificacion;
    int range;
    int rangeMaster;
    int estrategia;
    FILE * archivo;
    std::map<std::string, int> labels;
};

/*
*Estructura que almacena el resultado de cada
*hilo (el mapa de etiquetas actualizado).
*/
struct Result
{
    std::map<std::string, int> actualizado;
};


/*
*Estructura que contiene los datos necesarios
*de un hilo Lector.
*/
struct Data_Lector
{
    FILE * archivo;
    int identificacion;
    int argc;
    char ** argv;

};


/*
*Matodo encargado de realizar la tarea de los trabajadores.
*/
void * hiloTrabajador( void * args_void )
{
    FileReader * file;
    mutex = new Mutex();
    file = new FileReader();
    char * linea;
    int rango;

    struct Result * resultado = (struct Result*) malloc(sizeof *resultado);
    linea = (char *) calloc( 512, 1 );

    struct Data_Worker * datos = (struct Data_Worker*) args_void;
    int id = datos->identificacion;
    int strategy = datos->estrategia;
    FILE * fichero = datos->archivo;
    std::map<std::string, int> etiquetas = datos->labels;


    if(id == 0)
    {
        rango = datos->rangeMaster;
    }
    else
    {
        rango = datos->range;
    }

    mutex->Lock();

    if(busy == false)
    {
        busy = true;

        etiquetas = file->Estrategias(fichero, linea, etiquetas, rango, id, strategy);
    }

    busy = false;
    mutex->Unlock();
    resultado->actualizado = etiquetas;

    return resultado;
}


/*
*Metodo encargado de crear cierta cantidad de hilos trabajadores,
*segun la cantidad ingresada por el usuario.
*/
void * CrearTrabajadores( long cantidad, struct Data_Worker * trabajador )
{
    long hilo;
    pthread_t * worker;
    void * out;
    struct Result * resultado;

    worker = (pthread_t *) calloc( cantidad, sizeof( pthread_t ) );

    for ( hilo = 0; hilo < cantidad; hilo++ )
    {
        trabajador->identificacion = (int) hilo;
        pthread_create( & worker[ hilo ], NULL, hiloTrabajador, &trabajador );
    }


    for ( hilo = 0; hilo < cantidad; hilo++ )
    {
        pthread_join( worker[ hilo ], &out );
        resultado = (struct Result*) out;
        //Metodo de actualizacion
    }

    free( worker );
    return resultado;
}



/*
*
*/
void * hiloLector( void * args_void )
{
    FileReader * file;
    mutex = new Mutex();
    file = new FileReader();
    std::map<std::string, int> etiquetas;
    void * out;

    struct Data_Lector * datos = (struct Data_Lector*) args_void;
    struct Data_Worker * trabajador;
    struct Result * resultado = (struct Result*) malloc(sizeof *resultado);

    int id = datos->identificacion;
    FILE * fichero = datos->archivo;

    file->Read(datos->argc, datos->argv);

    //Se llenan los datos de la estructura a enviar.
    trabajador->range = file->getRange();
    trabajador->rangeMaster = file->getRangeMaster();
    trabajador->estrategia = file->getStrategy();
    trabajador->archivo = file->getFile();
    trabajador->labels = file->getMap();

    //Se crean los trabajadores. Cada uno hace su tarea y juntos retornan
    //el mapa de etiquetas actualizado.
    out = CrearTrabajadores (file->getWorkers(), trabajador);
    resultado = (struct Result *) out;
    etiquetas = resultado->actualizado;
    file->setMap(etiquetas);


    return resultado;
}


/*
*Metodo encargado de crear cierta cantidad de hilos Lectores,
*segun la cantidad de archivos html ingresados.
*/
void CrearLectores( long cantidad )
{
    long hilo;
    pthread_t * lector;

    lector = (pthread_t *) calloc( cantidad, sizeof( pthread_t ) );

    for ( hilo = 0; hilo < cantidad; hilo++ )
    {
        pthread_create( & lector[ hilo ], NULL, hiloLector, (void *) hilo );
    }


    for ( hilo = 0; hilo < cantidad; hilo++ )
    {
        pthread_join( lector[ hilo ], NULL );
    }

    free( lector );

}

/*
*Funcion principal.
*/
int main(int argc, char ** argv)
{
    FileReader archivo;

    pthread_t * lector;
    struct Data_Lector lect;
    void * out;
    struct Result * resultado;

    archivo.Read(argc, argv);
//
//    lect.identificacion = 0;
//    lect.archivo = archivo.getFile();
//    lect.argc = argc;
//    lect.argv = argv;
//
//    lector = (pthread_t *) calloc( 1 , sizeof( pthread_t ) );
//    pthread_create( & lector[ 0 ], NULL, hiloLector, &lect );
//    pthread_join( lector[ 0 ], &out );
//    resultado = (struct Result*) out;
//
//    archivo.setMap(resultado->actualizado);
//    archivo.imprimir(archivo.getMap());

}
