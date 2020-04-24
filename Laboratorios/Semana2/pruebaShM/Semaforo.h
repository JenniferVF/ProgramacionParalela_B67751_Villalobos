/*
   C++ class to encapsulate Unix semaphore intrinsic structures and
   system calls
*/

#define KEY 0xA12345	// Valor de la llave del recurso

class Semaforo {
   public:
      Semaforo( int ValorInicial = 0 );
      ~Semaforo();
      int Signal();	// Puede llamarse V
      int Wait();	// Puede llamarse P

   private:
      int id;		// Identificador del semaforo
};
