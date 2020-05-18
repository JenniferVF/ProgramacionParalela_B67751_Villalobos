#define KEY 0xA12345	// Valor de la llave del recurso

class Buzon {
   public:
      Buzon();
      ~Buzon();
      int Enviar( const char * mensaje, int veces, long tipo );	// Envia la tira de caracteres como un mensaje tipo
      //int Enviar( void * mensaje, int len, int veces, long tipo );
      int Recibir( char * mensaje, int * veces, long tipo );   // len es el tamaño máximo que soporte la variable mensaje
      //int Recibir( void * mensaje, int len, long tipo );   // mensaje puede ser de tipo msgbuf

   private:
      int id;		// Identificador del buzon
};
