#include "Semaforo.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

Semaforo::Semaforo(int valor)
{
    union uni
    {
        int val = 0;
    };
    uni u;
    id = semget(0xB67751, 1, IPC_CREAT|0600);
    u.val = valor; //Parametro
    int aux = semctl(id, 0, SETVAL, u);
}

Semaforo::~Semaforo()
{
    semctl(id, 0, IPC_RMID);
}

int Semaforo::Wait(int id)
{
    struct sembuf b;
    b.sem_num = 0;
    b.sem_op = -1;
    b.sem_flg = 0;

    int aux = semop(id, &b, 1);
    return aux;
}

int Semaforo::Signal(int id)
{
    struct sembuf b;
    b.sem_num = 0;
    b.sem_op = +1;
    b.sem_flg = 0;

    int aux = semop(id, &b, 1);
    return aux;
}

int Semaforo::getID(Semaforo *sem)
{
   return sem->id;
}
