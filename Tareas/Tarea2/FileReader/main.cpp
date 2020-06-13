#include <iostream>
#include <map>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <pthread.h>
#include "FileReader.h"
#include <ctime>
#include "Mutex.h"

using namespace std;

//Shared variables
Mutex * mutex;
//bool busy = false;
std::vector< std::string > archivos; //Almacena el archivo por leer en la casilla correspondiente al id de cada lector.
std::vector< fpos_t > positions; //Almacena la posicion del archivo por la que va cada lector.
std::vector< int > estrategias; //Almacena las estrategias de cada archivo.
std::vector< bool > semaforo; //Almacena los booleanos que actuan como semaforos.


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
*Estructura que contiene los datos necesarios
*de un hilo Lector.
*/
struct Data_Lector
{
    int identificacion;
    int trabajadores;
};


/*
*Estructura que contiene los datos necesitados
*por el hilo maestro.
*/
struct Data_Master
{
    int cant_arg;
    char ** argumentos;
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
    std::string name = archivos.at(idLec);
    //bool busy = semaforo.at(idLec);


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
        if(semaforo.at(idLec) == false)
        {
            mutex->Lock();
            semaforo.at(idLec) = true;
            pos = positions.at(idLec); //Se toma la posicion del archivo.

            //Realiza la estrategia que le corresponde
            recolecta = file->Estrategias(fichero, recolecta, rango, id, strategy, name, pos, contador);

            contador++;
            pos = file->getPos();
            positions.at(idLec) = pos; //Se actualiza el vector de posiciones
            finish = file->getFinish();

            semaforo.at(idLec) = false;
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

    file->Read(datos->trabajadores, estrategias.at(id), archivos.at(id));


    //Se llenan los datos de la estructura a enviar.
    trabajador.range = file->getRange();
    trabajador.rangeMaster = file->getRangeMaster();
    trabajador.estrategia = file->getStrategy();
    trabajador.idLector = id;
    trabajador.archivo = file->getFile();


    //Se actualiza el vector de posiciones
    positions.at(id) = file->getPos();

    //Se actualiza el vector semaforo
    semaforo.at(id) = false;

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
void * CrearLectores( long cantidad, struct Data_Lector read, std::map<std::string, int> recolecta)
{
    long hilo;
    pthread_t * lector;
    void * out;
    struct Result * resultado;
    std::map<std::string, int> actualizar;
    std::map<std::string, int>:: iterator it;
    std::string key;

    lector = (pthread_t *) calloc( cantidad, sizeof( pthread_t ) );

    for ( hilo = 0; hilo < cantidad; hilo++ )
    {
        read.identificacion = (int) hilo;
        printf("Crea Lector %li. \n", hilo);
        pthread_create( & lector[ hilo ], NULL, hiloLector, &read );
    }


    for ( hilo = 0; hilo < cantidad; hilo++ )
    {
        pthread_join( lector[ hilo ], &out );
        printf("Lector %li finaliza.\n", hilo);
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

    free( lector );
    resultado->actualizado = recolecta;
    return resultado;
}


/*
*Metodo encargado de realizar las tareas del HiloMaster.
*Distribuye los datos necesitados por los Lectores.
*/
void * hiloMaster( void * args_void )
{
    FileReader * file;
    file = new FileReader();

    std::map<std::string, int> recolecta;
    void * out;
    int cantidad; //Almacena la cantidad de archivos HTML ingresados

    struct Data_Master * maestro = (struct Data_Master*) args_void;
    struct Data_Lector datos;
    struct Result * resultado = (struct Result*) calloc(1, sizeof *resultado);


    if ( maestro->cant_arg < 4 )
    {
        printf( "Por favor ingrese: <cantidad de trabajadores> <estrategia(s) a utilizar (pegadas)> <archivo(s) a utilizar>.\n Estrategias: \n 0 - Un solo trabajador realiza la tarea.\n 1 - Total de lineas/cantidad de trabajadores. \n 2 - Grupos no contiguos de lineas. \n 3 - Entregar las lineas por demanda. \n 3 - Leer lineas por bloques. \n" );
        exit( 1 );
    }


    cantidad = (maestro->cant_arg)-3; //Cantidad de archivos ingresados.
    datos.trabajadores = atoi(maestro->argumentos[ 1 ]);

    archivos.resize(cantidad);
    positions.resize(cantidad);
    semaforo.resize(cantidad);
    estrategias.resize(cantidad, 0);


    //Se agregan los archivos ingresados por el usuario.
    int posicion = 0;
    for(int i = 3; i < maestro->cant_arg; i++)
    {
        std::string filename = maestro->argumentos[ i ];
        archivos.at(posicion) = filename;
        posicion++;
    }


    //Toma las estrategias y las agrega al vector.
    std::string num (maestro->argumentos[ 2 ]);
    int strategy = 0;

    //Se divide el string de las estrategias
    std::istringstream split(num);
    std::vector <std::string> digitos;
    std::string cambio;
    for(std::string each; std::getline(split, each, ','); digitos.push_back(each));

    //Se agregan las estrategias al vector.
    posicion = 0;
    for (int i = 0; i < digitos.size(); i++)
    {
        cambio = digitos[i];
        strategy = std::stoi(cambio);
        estrategias.at(posicion) = strategy;
        posicion++;
    }

    recolecta = file->inicializar(recolecta);
    out = CrearLectores((long)cantidad, datos, recolecta);

    resultado = (struct Result *) out;
    recolecta = resultado->actualizado;
    file->setMap(recolecta);

    printf("Hilo Master acumula los resultados.\n");
    return resultado;
}


/*
*Metodo encargado de imprimir en consola las etiquetas
*encontradas en el archivo. Asi como sus contadores.
*/
void imprimir(std::map<std::string, int> etiquetas, double time)
{
    std::map<std::string, int>:: iterator it = etiquetas.begin();
    char key[it->first.size() + 1];

    printf( "Lectura completada.\n" );
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

    printf( "Tiempo transcurrido: %f.\n", time );
}



/*
*Funcion principal.
*/
int main(int argc, char ** argv)
{

    pthread_t * master;
    struct Data_Master maestro;
    void * out;
    struct Result * resultado;

    clock_t start, finish; //Variables para medir el tiempo.
    double time = 0; //Resultado del tiempo transcurrido

    start = clock();
    maestro.cant_arg = argc;
    maestro.argumentos = argv;


    //Se crea el hilo maestro, encargado de crear a los Lectores.
    master = (pthread_t *) calloc( 1, sizeof( pthread_t ) );
    pthread_create( & master[ 0 ], NULL, hiloMaster, &maestro );
    printf("Crea hilo master.\n");
    pthread_join( master[ 0 ], &out );

    resultado = (struct Result*) out;

    finish = clock();
    time = finish-start;
    imprimir(resultado->actualizado, time);

}


