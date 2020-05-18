#include "Buzon.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

Buzon::Buzon()
{
    this->id = msgget(0xB67751, IPC_CREAT | 0600); //Identificador del buzon (cola de mensajes)
    //Para saber si falla
    if (-1 == id)
    {
        perror("Buzon::Buzon");
        exit(1);
    }

}

Buzon::~Buzon()
{
    int status;
    status = msgctl(id, IPC_RMID, NULL);
    //Para saber si falla
    if (status == -1)
    {
        perror("Buzon::~Buzon");
        exit(1);
    }
}

int Buzon::Enviar(const char * mensaje, int veces, long tipo)
{
    int status;
    struct msgbuf
    {
        long int mtype;		// Tipo de mensaje, debe ser >0
        char mtext[80];
    } mens;
    mens.mtype = tipo;

    strncpy(mens.mtext, mensaje, strlen(mensaje));
    status = msgsnd(id, & mens, sizeof(mens.mtext), IPC_NOWAIT); //La bandera tambien puede declararse en 0.
    if (status == -1)
    {
        perror("Buzon::Enviar");
        exit(1);
    }
    return status;
}

int Buzon::Recibir(char * mensaje, int * veces, long tipo)
{
    int status;
    struct msgbuf
    {
        long int mtype;		// Tipo de mensaje, debe ser >0
        char mtext[80];
    } mens;

    mens.mtype = tipo;

    status = msgrcv(id, & mens, sizeof(char), tipo, IPC_NOWAIT); //La bandera tambien puede declararse en 0.
    if (status == -1)
    {
        perror("Buzon::Recibir");
        exit(1);
    }
    return status;
}

//int Buzon::Enviar(void * mensaje, int len, int veces, long tipo)
//{
//    int status;
//    struct msgbuf
//    {
//        long mtype;		// Tipo de mensaje, debe ser >0
//        char mtext[80];
//    };
//
//    struct msgbuf mens;
//    mens.mtype = 1;
//
//    strncpy(mens.mtext, (const char*)mensaje, strlen((const char*)mensaje));
//    status = msgsnd(id, & mens, sizeof(mens.mtext), IPC_NOWAIT); //La bandera tambien puede declararse en 0.
//    if (status == -1)
//    {
//        perror("Buzon::Enviar");
//        exit(1);
//    }
//    return status;
//}
//
//int Buzon::Recibir(void * mensaje, int len, long tipo)
//{
//    int status;
//    struct msgbuf
//    {
//        long mtype;		// Tipo de mensaje, debe ser >0
//        char mtext[80];
//    };
//
//    struct msgbuf mens;
//    //mens.mtype = 1;
//
//    status = msgrcv(id, & mens, sizeof(mens.mtext), 1, IPC_NOWAIT); //La bandera tambien puede declararse en 0.
//    if (status == -1)
//    {
//        perror("Buzon::Recibir");
//        exit(1);
//    }
//    return status;
//}
