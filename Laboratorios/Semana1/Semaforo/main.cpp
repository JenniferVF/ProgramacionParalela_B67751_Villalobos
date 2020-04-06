#include <iostream>
#include "Semaforo.h"

using namespace std;

int main()
{
    Semaforo *sem = new Semaforo();

    //Para saber si falla
    int id = sem->getID(sem);
    if (id == -1)
    {
        perror("Semaforo::Semaforo");
        exit(1);
    }


    delete sem;
}
