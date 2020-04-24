#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <string.h>
#include <wait.h>

#include "Semaforo.h"

int main() {

   int i;
   Semaforo s;
   int id = shmget( 123456, 1024, 0700 | IPC_CREAT );
   char * area = (char *) shmat( id, NULL, 0 );
   char * var = "Area de memoria compartida 2020";
   char newvar[100];

   if ( fork() ) {
      strcpy( area, var );
      printf( "Escriba un numero y <ENTER> para continuar ...\n" );
      scanf( "%i", &i );
      printf( "Variable es %s \n", area );
      s.Signal();
   }
   else {
      s.Wait();
      strcpy( newvar, area );
      printf( "La variable compartida es: %s \n", newvar );
      exit( 0 );
   }
   wait( &i );
   printf( "Fin del programa ... \n" );
   shmdt( area );
   shmctl( id, IPC_RMID, NULL );
}
