/*
 * Suma uno a un total mil veces por cada proceso generado
 * Recibe como parametro la cantidad de procesos que se quiere arrancar
 * Controla el acceso a la variable compartida por medio de un semaforo
 *
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
#include <sys/sem.h>
#include <time.h>

/*
  Definicion de la estructura para inicializar los semaforos
*/
union semun {
   int              val;    /* Value for SETVAL */
   struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
   unsigned short  *array;  /* Array for GETALL, SETALL */
   struct seminfo  *__buf;  /* Buffer for IPC_INFO
                               (Linux-specific) */
};

/*
   Realiza la suma
*/
double SumarUno( long * suma ) {
   int i;

   for ( i = 0; i < 1000; i++ ) {
      (* suma)++;			// Suma uno
//      usleep( 1 );
   }
   exit( 0 );
}


/*
 * Realiza la suma controlando "race condition"
 * Los procesos acceden a la variable en orden, controlados por un semaforo
 */
double SumarUnoControlado( int semid, long * suma ) {
   int i;
   struct sembuf pedir, devolver;

   pedir.sem_num = 0;
   pedir.sem_op = -1;
   pedir.sem_flg = 0;
   semop( semid, &pedir, 1 );	// Solicita paso a traves del semaforo
   for ( i = 0; i < 1000; i++ ) {
      (* suma)++;			// Suma uno
//      usleep( 1 );
   }

   devolver.sem_num = 0;
   devolver.sem_op = +1;
   devolver.sem_flg = 0;
   semop( semid, &devolver, 1 );	// Libera el semaforo para que otros procedan

   exit( 0 );

}


/*
 * Serial test
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
 * Fork test with race condition
 */
int ForkTestRaceCondition( int procesos, long * total ) {
   int proceso, pid;

   for ( proceso = 0; proceso < procesos; proceso++ ) {
      pid = fork();
      if ( ! pid ) {
         SumarUno( total );
      }
   }

   for ( proceso = 0; proceso < procesos; proceso++ ) {
      int status;
      pid_t pid = wait( &status );
   }

}


/*
 * Fork test with NO race condition
 * Los procesos acceden a la variable en orden
 */
int ForkTestNORaceCondition( int procesos, int semid, long * total ) {
   int proceso, pid;

   for ( proceso = 0; proceso < procesos; proceso++ ) {
      pid = fork();
      if ( ! pid ) {
         SumarUnoControlado( semid, total );
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
   union semun valores;

// Create semaphore array, you can use your class
   semid = semget( 0xA12345, 1, IPC_CREAT | 0600 );
   valores.val = 1;
   semctl( semid, 0, SETVAL, valores );

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

   start = clock();
   * total = 0;
   ForkTestNORaceCondition( procesos, semid, total );
   finish = clock();
   used = ((double) (finish - start)) / CLOCKS_PER_SEC;
   printf( "Fork, No Race Cond.: Valor acumulado es \033[91m %ld \033[0m con %ld procesos in %f seconds\n", * total, procesos, used );


// Destroy semaphores
   semctl( semid, 1, IPC_RMID );

// Destroy shared memory segment
   shmdt( total );
   shmctl( shmid, IPC_RMID, 0 );

}
