/*
 * Suma uno a un total mil veces por cada proceso generado
 * Recibe como parametro la cantidad de procesos que se quiere arrancar
 * Author: Programacion Concurrente (Francisco Arroyo)
 * Version: 2020/Abr/11
 */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

/*
 *  Do some work, by now add one to a variable
 */
double AddOne( long * suma ) {
   int i;

   for ( i = 0; i < 1000; i++ ) {
      usleep( 1 );
      (* suma)++;			// Suma uno
   }
   exit( 0 );
}


/*
  Serial test
*/
int SerialTest( int procesos, long * total ) {
   int i, proceso;

   for ( proceso = 0; proceso < procesos; proceso++ ) {

      for ( i = 0; i < 1000; i++ ) {
         (* total)++;			// Suma uno
//         usleep( 1 );
      }

   }

}


/*
  Fork test with race condition
*/
int ForkTestRaceCondition( int procesos, long * total ) {
   int proceso, pid;

   for ( proceso = 0; proceso < procesos; proceso++ ) {
      pid = fork();
      printf("Soy el proceso padre.\n");
      if ( ! pid ) {
      printf("Soy el proceso hijo.\n");
         AddOne( total );
      }
   }

   for ( proceso = 0; proceso < procesos; proceso++ ) {
      int status;
      pid_t pid = wait( &status );
   }

}


int main( int argc, char ** argv ) {
   long procesos;
   long * total;
   int proceso, pid, shmid, semid;
   clock_t start, finish;
   double used;

// Create shared memory segment
   shmid = shmget( 0xA12345, sizeof( long ), IPC_CREAT | 0600 );
   total = (long *) shmat( shmid, NULL, 0 );

   procesos = 100;
   if ( argc > 1 ) {
      procesos = atol( argv[ 1 ] );
   }

   start = clock();
   * total = 0;
   SerialTest( procesos, total );
   finish = clock();
   used = ((double) (finish - start)) / CLOCKS_PER_SEC;
   printf( "Serial version:      Valor acumulado es \033[91m %ld \033[0m con %ld procesos in %f seconds\n", * total, procesos, used );

   start = clock();
   * total = 0;
   ForkTestRaceCondition( procesos, total );
   finish = clock();
   used = ((double) (finish - start)) / CLOCKS_PER_SEC;
   printf( "Fork, Race Cond.:    Valor acumulado es \033[91m %ld \033[0m con %ld procesos in %f seconds\n", * total, procesos, used );

// Destroy shared memory segment
   shmdt( total );
   shmctl( shmid, IPC_RMID, 0 );

}
