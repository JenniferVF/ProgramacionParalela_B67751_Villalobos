/*
  Calcula el numero PI utilizando una serie "infinita"
  Debe recibir la cantidad de iteraciones como parametro
  pi = (-1)^i x 4/(2xi + 1)
 * Semana 02, deben crear dos programas:
 *    Agregar Semaforos y un segmento de memoria compartida para lograr la solución correcta
 *    Agregar Paso de mensajes para lograr la solución correcta
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sstream>
#include <string.h>
#include <iostream>

//Librerias para compartir memoria
#include "Semaforo.h"
#include <sys/shm.h>
using namespace std;

/*
   Realiza la acumulacion de terminos desde una posicion inicial hasta un termino final
*/
double calcularSumaParcialPi( double Pi[], int proceso, long inicial, long terminos, char* acum)
{
    double casiPi = 0;
    double alterna = 4;
    long divisor = 0;
    long termino;
    std::stringstream ss;

    for ( termino = inicial; termino < terminos; termino++ )
    {
        divisor = termino + termino + 1;		// 2 x termino + 1
        casiPi += alterna/divisor;		// 4 / (2xi + 1)
        alterna *= -1;				// Pasa de 4 a -4 y viceversa, para realizar la aproximacion de los terminos
    }
    //Pi[ proceso ] = casiPi;			// Guarda el resultado en el vector y finaliza
    ss << casiPi;
    const char* str = ss.str().c_str();
    strcpy(acum, str);
    exit( 0 );
}


int main( int argc, char ** argv )
{
    long terminos, inicio, fin;
    int proceso;
    int pid;
    double casiPi[ 10 ] = { 0 };

    //Variables para compartir memoria
    Semaforo sem;
    int id = shmget(123456, 1024, 0700|IPC_CREAT);
    char* acum = (char*)shmat(id, NULL, 0);
    char* acumant;
    char var[100];
    int resultado;
    int pactual;
    char poh = 'p';

    //strcpy(acumant, acum);
    terminos = 1000000;
    if ( argc > 1 )
    {
        terminos = atol( argv[ 1 ] );
    }

    for ( proceso = 0; proceso < 10; proceso++ )
    {
        cout << poh;
        cout << acum;
        inicio = proceso * terminos/10;
        fin = (proceso + 1) * terminos/10;
        pid = fork();

        if ( poh == 'h' )   //Si ya un proceso hijo hizo el calculo
        {
        cout << acum;
        strcpy(var, acum); //Lo obtenido en acum, se pasa a var
        resultado = atof(var); //Var se convierte a double
        casiPi[ pactual ] = resultado; //Se ingresa el valor al array
        strcpy(acumant, acum); //Se vuelven a establecer los valores.
        }

        if ( ! pid )   //Si es un hijo
        {
            //sem.Wait();
            poh = 'h';
            pactual = proceso;
            calcularSumaParcialPi( casiPi, proceso, inicio, fin, acum);
        }
        else     //Si es el padre
        {
            poh = 'p';
            //Si el acumulador ha cambiado de valor... un proceso hijo finalizo
            if (strcmp(acum, acumant) != 0)
            {
                poh = 'h';
            }
            printf("Creating process %d: starting value %ld, finish at %ld\n", pid, inicio, fin );
            //sem.Signal();
        }

    }

    for ( proceso = 0; proceso < 10; proceso++ )
    {
        int status;
        pid_t pid = wait( &status );
    }

    for ( proceso = 1; proceso < 10; proceso++ )
    {
        cout << "proceso " << proceso <<": " << casiPi[proceso];
        casiPi[ 0 ] += casiPi[ proceso ];
    }

    printf( "Valor calculado de Pi es \033[91m %g \033[0m con %ld terminos\n", casiPi[0], terminos );


}
