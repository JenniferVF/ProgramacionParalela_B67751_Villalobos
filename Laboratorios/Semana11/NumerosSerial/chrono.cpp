/*  Implantación para realizar la medición de tiempo
 *
 *  Autor: Programacion Paralela y Concurrente
 *  Fecha: 2020/Jun/15
 *
 */

#include <time.h>
#include "chrono.h"

/*
 *  Inits a instance variable to zero
 */
Chrono::Chrono() {
   this->ts.tv_sec = 0;
   this->ts.tv_nsec = 0;
}


/*
 *  Copy contructor
 */
Chrono::Chrono( struct timespec & t ) {

   this->ts.tv_sec = t.tv_sec;
   this->ts.tv_nsec = t.tv_nsec;

}


/*
 *  Class destructor
 */
Chrono::~Chrono() {
}


/*
 *  Stores actual time in instance variable
 */
int Chrono::readTime() {
   int status;
   struct timespec t;

   status = clock_gettime( CLOCK_MONOTONIC, &t );
   this->ts.tv_sec = t.tv_sec;
   this->ts.tv_nsec = t.tv_nsec;

   return status;

}


/*
 *  Get number of seconds from instance variable
 */
int Chrono::getSecs() {

   return this->ts.tv_sec;

}


/*
 *  Gets number of nano-seconds from instance variable
 */
int Chrono::getnSecs() {

   return this->ts.tv_nsec;

}


/*
 *  Assignment operator
 */
Chrono & Chrono::operator = ( const Chrono &rhs ) {

   if ( this != &rhs ) {
      this->ts.tv_sec = rhs.ts.tv_sec;
      this->ts.tv_nsec = rhs.ts.tv_nsec;
   }

   return *this;

}


/*
 *  Substract operator
 */
Chrono & Chrono::operator - ( const Chrono &rhs ) {

   if ( this != &rhs ) {
      if ( (this->ts.tv_nsec - rhs.ts.tv_nsec) < 0) {	// nanoseconds difference is negative
         this->ts.tv_sec = this->ts.tv_sec - rhs.ts.tv_sec - 1;	// Substract one from seconds count
         this->ts.tv_nsec = 1000000000 + this->ts.tv_nsec - rhs.ts.tv_nsec;	// Add one second to diff
      } else {
         this->ts.tv_sec -= rhs.ts.tv_sec;
         this->ts.tv_nsec -= rhs.ts.tv_sec;
      }
   }

   return *this;
}


/*
 *  Substract and assingment operator
 */
Chrono & Chrono::operator -= ( const Chrono &rhs ) {

   return *this - rhs;
}


/*
 * CI0117 - 2020-i
 * Prof. Francisco Arroyo
 *
 * Dejamos los demás métodos de la clase como ejercicios para el usuario
 *
 */
