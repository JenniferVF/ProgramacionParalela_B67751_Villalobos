#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

const char * html_labels[] = {
	"abc",
	"bcd",
	"cdef",
	"defg",
	"efgh",
	"li",
	""
};

int main( int argc, char ** argv ) {
   int id, i, size, st;
   struct msgbuf {
      long mtype;
      int veces;
      char etiqueta[ 32 ];
   } mensaje;

   id = msgget( 0xA12345, IPC_CREAT | 0600 );
   if ( -1 == id ) {
      perror( "Enviar sin clases" );
      exit( 1 );
   }

   i = 0;
   while ( ( size = strlen( html_labels[ i ] ) ) > 0 ) {
      mensaje.mtype = 2020;
      mensaje.veces = i;
      strncpy( mensaje.etiqueta, html_labels[ i ], size );
      st = msgsnd( id, (const void *) & mensaje, sizeof( mensaje ), IPC_NOWAIT );
      printf( "Label: %s, status %d \n", html_labels[ i ], st );
      i++;
   }

//   msgctl( id, IPC_RMID, NULL );

}
