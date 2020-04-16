/*
   C++ class to encapsulate Unix semaphore intrinsic structures and
   system calls
*/

#define KEY 0xA12345	// Valor de la llave del recurso

class Semaforo {
   public:
      Semaforo( int ValorInicial = 0 );
      ~Semaforo();
      int Signal(int id);	// Puede llamarse V
      int Wait(int id);	// Puede llamarse P
      int getID(Semaforo sem);  // Obtiene el identificador
   private:
      int id;		// Identificador del semaforo
};
