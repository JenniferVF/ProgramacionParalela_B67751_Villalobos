#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <stdlib.h>
#include <pthread.h>
#include "FileReader.h"
#include "Mutex.h"

using namespace std;

//Shared variables
Mutex * mutex;
bool busy = false;
std::vector< std::string > archivos; //Almacena el archivo por leer en la casilla correspondiente al id de cada lector.


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
*Metodo encargado de realizar la tarea de los trabajadores.
*/
void * hiloTrabajador( void * args_void )
{

    FileReader * file;
    mutex = new Mutex();
    file = new FileReader();
    int rango;

    struct Result * resultado = (struct Result*) calloc(1, sizeof *resultado);
    resultado->actualizado = file->inicializar(resultado->actualizado);
    struct Data_Worker * datos = (struct Data_Worker*) args_void;


    int id = datos->identificacion;
    int strategy = datos->estrategia;
    FILE * fichero;  //Es necesario?
    std::map<std::string, int> recolecta = file->inicializar(recolecta);
    std::string name = archivos.back();

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
        //Realiza la estrategia que le corresponde
        recolecta = file->Estrategias(fichero, recolecta, rango, id, strategy, name);
    }

    busy = false;
    mutex->Unlock();
    resultado->actualizado = recolecta;

    return resultado;
}


/*
*Metodo encargado de crear cierta cantidad de hilos trabajadores,
*segun la cantidad ingresada por el usuario.
*/
void * CrearTrabajadores( long cantidad, struct Data_Worker * trabajador, std::map<std::string, int> recolecta)
{
    long hilo;
    pthread_t * worker;
    void * out;
    struct Result * resultado;
    std::map<std::string, int> actualizar;
    std::map<std::string, int>:: iterator it;
    std::string key;

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
        actualizar = resultado->actualizado;

        //Metodo de actualizacion del mapa de etiquetas del lector
        it = recolecta.begin();
        while(it != recolecta.end())
        {
            key = it->first;
            recolecta[key] = recolecta[key] + actualizar[key];
            it++;
        }
    }

    free( worker );
    resultado->actualizado = recolecta;
    return resultado;
}



/*
*Metodo encargado de realizar la tarea de los maestro_lector.
*/
void * hiloLector( void * args_void )
{

    FileReader * file;
    mutex = new Mutex();
    file = new FileReader();
    std::map<std::string, int> recolecta = file->inicializar(recolecta);
    void * out;

    struct Data_Lector * datos = (struct Data_Lector*) args_void;
    struct Data_Worker * trabajador = (struct Data_Worker*) calloc(1, sizeof *trabajador);
    struct Result * resultado = (struct Result*) calloc(1, sizeof *resultado);

    int id = datos->identificacion;

    file->Read(datos->argc, datos->argv);


    //Se llenan los datos de la estructura a enviar.
    trabajador->range = file->getRange();
    trabajador->rangeMaster = file->getRangeMaster();
    trabajador->estrategia = file->getStrategy();
    trabajador->archivo = file->getFile();

    //Se crean los trabajadores. Cada uno hace su tarea y juntos retornan
    //el mapa de etiquetas actualizado.
    out = CrearTrabajadores (file->getWorkers(), trabajador, recolecta);
    resultado = (struct Result *) out;
    recolecta = resultado->actualizado;
    file->setMap(recolecta);

    return resultado;
}


/*
*Metodo encargado de crear cierta cantidad de hilos Lectores,
*segun la cantidad de archivos html ingresados.
*/
//void CrearLectores( long cantidad )
//{
//    long hilo;
//    pthread_t * lector;
//
//    lector = (pthread_t *) calloc( cantidad, sizeof( pthread_t ) );
//
//    for ( hilo = 0; hilo < cantidad; hilo++ )
//    {
//        pthread_create( & lector[ hilo ], NULL, hiloLector, (void *) hilo );
//    }
//
//
//    for ( hilo = 0; hilo < cantidad; hilo++ )
//    {
//        pthread_join( lector[ hilo ], NULL );
//    }
//
//    free( lector );
//
//}


/*
*Metodo encargado de imprimir en consola las etiquetas
*encontradas en el archivo. Asi como sus contadores.
*/
void imprimir(std::map<std::string, int> etiquetas)
{
    std::map<std::string, int>:: iterator it = etiquetas.begin();
    char key[it->first.size() + 1];

    printf( "Lectura del archivo completada.\n" );
    while(it != etiquetas.end())
    {

        //Si el contador de la etiqueta es mayor a cero, es decir, aparecio al menos una vez, se imprime
        if(etiquetas[it->first] > 0)
        {
            strcpy(key, it->first.c_str());
            printf("Etiqueta: %s. Cantidad de veces encontrada: %d.\n", key, etiquetas[key]);
        }
        it++;
    }
}



/*
*Funcion principal.
*/
int main(int argc, char ** argv)
{

    pthread_t * lector;
    struct Data_Lector lect;
    void * out;
    struct Result * resultado;

    std::string filename = argv[1];
    archivos.push_back(filename);


    lect.identificacion = 0;

    lect.argc = argc;
    lect.argv = argv;

    lector = (pthread_t *) calloc( 1, sizeof( pthread_t ) );
    pthread_create( & lector[ 0 ], NULL, hiloLector, &lect );
    pthread_join( lector[ 0 ], &out );

    resultado = (struct Result*) out;
    imprimir(resultado->actualizado);

}


