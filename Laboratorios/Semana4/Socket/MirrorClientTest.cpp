/*
 *
 */
#include <stdio.h>
#include "Socket.h"


int main( int argc, char ** argv ) {
   Socket s;     // Crea un socket de IPv4, tipo "stream"
   char buffer[ 512 ];

   s.Connect( "127.0.0.1", 9876 ); // Same port as server
   s.Write( argv[1] );		// Send first program argument to server
   s.Read( buffer, 512 );	// Read the answer sent back from server
   printf( "%s", buffer );	// Print the string

}
