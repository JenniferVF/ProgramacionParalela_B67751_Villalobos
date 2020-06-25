/*  Interfaz para realizar la medición de tiempo
 *
 *  Autor: Programacion Paralela y Concurrente
 *  Fecha: 2020/Jun/15
 *
 * Utilización
 *    #include "chrono.h"
 *    ...
 *    Chrono inicio, fin;
 *    ...
 *    inicio.readTime();
 *    // Código a medir el tiempo
 *    fin.readTime();
 *    ...
 *    fin -= inicio;
 *    printf( "Time taken: %ld sec., %ld ns\n", fin.getSecs(), fin.getnSecs() );
 *
 */


#include <stdlib.h>

class Chrono{

    public:
	Chrono();
	Chrono( struct timespec & );
	~Chrono();
	int readTime();	// Reads system time
	int getSecs();	// Returns the seconds part of time in variable
	int getnSecs();	// Returns the nanoseconds part of time in variable
	Chrono & operator =  ( const Chrono & rhs );
	Chrono & operator +  ( const Chrono & rhs );
	Chrono & operator -  ( const Chrono & rhs );
	Chrono & operator += ( const Chrono & rhs );
	Chrono & operator -= ( const Chrono & rhs );

    private:
	struct timespec ts;
};
