#include <stdlib.h>
#include <pthread.h>

#include "Mutex.h"

Mutex::Mutex() {
   int resultado;
   pthread_mutexattr_t * atributos;

   this->mutex = (pthread_mutex_t *) calloc( 1, sizeof( pthread_mutex_t ) );
   resultado = pthread_mutex_init( this->mutex, NULL );
   if ( 0 != resultado ) {
      exit( resultado );
   }

}


Mutex::~Mutex() {
   int resultado;

   resultado = pthread_mutex_destroy( this->mutex );
   if ( 0 != resultado ) {
      exit( resultado );
   }

   free( this->mutex );

}

int Mutex::Lock() {
   int resultado;

   resultado = pthread_mutex_lock( this->mutex );
   if ( 0 != resultado ) {
      exit( resultado );
   }

   return resultado;

}


/*
 *  If trying to lock an already locked mutex this call will return EBUSY (>0)
 *  Calling thread must check if lock was acquired or it must retry
 *  If mutex is unlocked this call will return 0 and calling thread must proceed
 *
 *  Warning: Other errors are not handled and are passed to calling thread
 */
int Mutex::TryLock() {
   int resultado;

   resultado = pthread_mutex_trylock( this->mutex );

   return resultado;

}


int Mutex::Unlock() {
   int resultado;

   resultado = pthread_mutex_unlock( this->mutex );
   if ( 0 != resultado ) {
      exit( resultado );
   }

   return resultado;

}
