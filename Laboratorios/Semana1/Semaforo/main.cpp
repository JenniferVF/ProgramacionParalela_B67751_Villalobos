#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "Semaforo.h"

using namespace std;

int main()
{
/*
    Semaforo sem;

    //Para saber si falla
    int id = sem->getID(sem);
    if (id == -1)
    {
        perror("Semaforo::Semaforo");
        exit(1);
    }


    delete sem;*/

    Semaforo s;
    char * texto = NULL;
    int i;
    int id = s.getID(s); //Se obtiene la identificacion del semaforo

    if ( fork() ) {
       for (int i = 0; i < 10; i++ )
           printf( "Esperando para activar el semaforo %d \n", i );
//     printf( "%s\n", texto );
       scanf( "%i", &i );
       s.Signal(id);
    }
    else {
       printf( "Esperando que el semaforo se active  ...\n" );
       s.Wait(id);
       printf( "Semaforo activado\n" );
    }
}
/*
   Salida esperada:
     Esperando para activar el semaforo ... (puede aparecer despues)
     Esperando que el semaforo se active
     Esperando para activar el semaforo ...
     Esperando para activar el semaforo ...
     ...
     [Debe escribir un numero para lectura por scanf ...]
     Semaforo activado
*/
