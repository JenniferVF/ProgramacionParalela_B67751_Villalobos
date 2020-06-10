#include <iostream>
#include <map>
#include <stdio.h>
#include <unistd.h>
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
std::vector< fpos_t > positions(1); //Almacena la posicion del archivo por la que va cada lector.

/*
*Estructura que almacena los datos necesitados
*por los hilos trabajadores.
*/
struct Data_Worker
{
    int identificacion;
    int idLector = 0;
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
    int id;
    int idLec;
    int strategy;
    fpos_t pos; //Variable que guarda la posicion en el archivo.
    FILE * fichero;  //Es necesario?
    bool finish = false;  //Variable que indica si el hilo ya finalizo la lectura.
    int contador = 0; //En caso de que sea la estrategia 1.

    struct Result * resultado = (struct Result*) calloc(1, sizeof *resultado);
    resultado->actualizado = file->inicializar(resultado->actualizado);
    struct Data_Worker * datos = (struct Data_Worker*) args_void;


    id = datos->identificacion;
    idLec = datos->idLector;
    strategy = datos->estrategia;
    fichero = datos->archivo;

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


    do
    {
        if(busy == false)
        {
            mutex->Lock();
            busy = true;
            pos = positions.at(idLec); //Se toma la posicion del archivo.

            //Realiza la estrategia que le corresponde
            recolecta = file->Estrategias(fichero, recolecta, rango, id, strategy, name, pos, contador);

            contador++;
            pos = file->getPos();
            positions.at(idLec) = pos; //Se actualiza el vector de posiciones
            finish = file->getFinish();

            busy = false;
            mutex->Unlock();
        }
        else
        {
            usleep(50);
        }
    }
    while(finish == false);


    resultado->actualizado = recolecta;

    return resultado;
}


/*
*Metodo encargado de crear cierta cantidad de hilos trabajadores,
*segun la cantidad ingresada por el usuario.
*/
void * CrearTrabajadores( long cantidad, struct Data_Worker trabajador, std::map<std::string, int> recolecta)
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
        trabajador.identificacion = (int) hilo;
        printf("Crea trabajador %li. \n", hilo);
        pthread_create( & worker[ hilo ], NULL, hiloTrabajador, &trabajador );
    }


    for ( hilo = 0; hilo < cantidad; hilo++ )
    {
        pthread_join( worker[ hilo ], &out );
        printf("Trabajador %li finaliza.\n", hilo);
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
    struct Data_Worker trabajador;
    struct Result * resultado = (struct Result*) calloc(1, sizeof *resultado);

    const int id = datos->identificacion;

    file->Read(datos->argc, datos->argv);


    //Se llenan los datos de la estructura a enviar.
    trabajador.range = file->getRange();
    trabajador.rangeMaster = file->getRangeMaster();
    trabajador.estrategia = file->getStrategy();
    trabajador.idLector = id;
    trabajador.archivo = file->getFile();


    //Se actualiza el vector de posiciones
    positions.at(id) = file->getPos();


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


