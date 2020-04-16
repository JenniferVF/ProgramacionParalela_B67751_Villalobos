#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include "Semaforo.h"

union uni
{
    int val = 0;   //Valor para el comando SETVAL
    struct semid_ds *buf; //Para los comandos IPC_SET
    unsigned short *array; //Para los comandos GETALL y SETALL
    struct seminfo *__buf; //Para el comando IPC_INFO
};

//Inicializacion de la union y la estructura
uni u;
struct sembuf b; //Utilizado para el Wait y Signal

Semaforo::Semaforo(int valor)
{
    this->id = semget(0xB67751, 1, IPC_CREAT|0600);

    //Para saber si falla
    if (id == -1)
    {
        perror("Semaforo::Semaforo");
        exit(1);
    }

    u.val = valor; //Parametro entrante (0)
    int aux = semctl(id, 0, SETVAL, u);
}

Semaforo::~Semaforo()
{
    semctl(id, 0, IPC_RMID);
}

int Semaforo::Wait()
{
    b.sem_num = 0;
    b.sem_op = -1;
    b.sem_flg = 0;

    int aux = semop(id, &b, 1);
}

int Semaforo::Signal()
{
    b.sem_num = 0;
    b.sem_op = +1;
    b.sem_flg = 0;

    int aux = semop(id, &b, 1);
}
