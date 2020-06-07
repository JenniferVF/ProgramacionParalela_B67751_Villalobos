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
struct Data
{
    int identificacion;
    int range;
    int rangeMaster;
    int estrategia;
    char * line;
    FILE * archivo;
    std::map<std::string, int> labels;
};

/*
*Estructura que almacena el resultado de cada
*hilo trabajador.
*/
struct Result
{
    std::map<std::string, int> actualizado;
};


/*
*
*/
void * hiloTrabajador( void * args_void )
{
    FileReader * file;
    mutex = new Mutex();
    file = new FileReader();

    struct Result * resultado = (struct Result*) malloc(sizeof *resultado);

    struct Data * datos = (struct Data*) args_void;
    int id = datos->identificacion;
    int strategy = datos->estrategia;
    char * linea = datos->line;
    FILE * fichero = datos->archivo;
    std::map<std::string, int> etiquetas = datos->labels;
    int rango;

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

        etiquetas = file->Estrategias(fichero, linea, etiquetas, rango, id);
    }
    mutex->Unlock();
    resultado->actualizado = etiquetas;

    return resultado;
}

int main(int argc, char ** argv)
{
    FileReader archivo;

    //archivo.inicializar(etiquetas);

    archivo.Read(argc, argv);

}
