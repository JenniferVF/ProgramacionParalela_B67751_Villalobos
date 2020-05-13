#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

using namespace std;

int Collatz(int);

int  main(int  argc, char *argv[])
{
    int limite; //Numero elegido por el usuario
    int cant_pasos; //Cantidad e pasos en que se realizo la conjetura de Collatz.
    int master_id; //El id del proceso padre
    int son_id; //El id del (de los) proceso(s) hijo(s)
    int cant_procs = 5; //Cantidad de procesos por crear
    int rango; //El rango que le corresponde a cada proceso hijo
        int start; //Inicio del rango
    int top; //Fin del rango

    int    ShmID;
    int    *ShmPTR;
    pid_t  pid;
    int    status;

    //Se le pide al usuario que ingrese un numero.
    if (argc != 2)
    {
        printf("Ingrese: %s <cantidad limite> (que sea potencia de 10)\n", argv[0]);
        exit(1);
    }

    //if(atoi(argv[1]) ){}

    //Se define el rango: 2, limite.
    limite = atoi(argv[1]);
    rango = limite/cant_procs;
    //Se establece en cero los elementos de la memoria compartida
    ShmPTR[0] = 0;
    ShmPTR[1] = 0;

    //Se crea el segmento de memoria compartida
    ShmID = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);
    if (ShmID < 0)
    {
        perror("Error: shmget\n");
        exit(1);
    }


    //Se crean los procesos hijos.
    master_id = getpid(); //Se toma el id del proceso padre

    for(int i=0; i < cant_procs; i++)
    {
        if(getpid() == master_id)
        {
            pid = fork();
            if (pid < 0)
            {
                perror("Error: fork\n");
                exit(1);
            }
        }
        else
        {
            break;
        }
    }

    //Cada hijo se une a la memoria compartida
    ShmPTR = (int *) shmat(ShmID, NULL, 0);
    if ((intptr_t) ShmPTR == -1)
    {
        perror("Error: shmat\n");
        exit(1);
    }


    //Cada hijo hace su parte del calculo.
    if (pid == 0)
    {
        //Se obtiene el id del proceso hijo.
        son_id = (getpid()-master_id)-1;

        //Se define el inicio y el final del rango
        //en el que se va a realizar el calculo.
        //Esto segun el id del proceso.
        start = (rango * son_id) + 1;
        top = rango * (son_id+1);

        //Si es el proceso 0, el inicio debe tomarse
        //a partir de 2.
        if(son_id == 0)
        {

            for(int i = 2; i <= top; i++)
            {
                cant_pasos = Collatz(i);
                if(cant_pasos > ShmPTR[1])
                {
                    ShmPTR[0] = i;
                    ShmPTR[1] = cant_pasos;
                }
            }
        }
        //Si son los demas procesos, el inicio se toma normal.
        else
        {

            for(int i = start; i <= top; i++)
            {
                cant_pasos = Collatz(i);
                if(cant_pasos > ShmPTR[1])
                {
                    ShmPTR[0] = i;
                    ShmPTR[1] = cant_pasos;
                }
            }
        }
    }

    //Cada hijo se separa de la memoria compartida.
    if(shmdt((void *) ShmPTR) == -1)
    {
        perror("Error: shmdt\n");
    }
//    else
//    {
//        printf("Process %d detached the segment\n", getpid());
//    }

    //Todos los procesos hijo terminan. El padre espera.
    if(getpid() == master_id)
    {
        for(int i=0; i < cant_procs; i++)
        {
            wait(&status);
        }
    }
    else
    {
        exit(0);
    }

    //Se anexa el puntero al proceso master.
    ShmPTR = (int*) shmat(ShmID, NULL, 0);
    //Se muestra el resultado
    printf("Limite superior indicado: %d \nElemento del conjunto con mas pasos: %d \nPasos realizados: %d \n\n",
           limite, ShmPTR[0], ShmPTR[1]);

    //El proceso master elimina la memoria compartida.
    shmctl(ShmID, IPC_RMID, NULL);
    printf("Server exits...\n");
    exit(0);
}

/*
*Metodo encargado de realizar la conjetura de
*Collatz con el termino que ingresa como parametro.
*/
int Collatz(int rango)
{
    int cant_pasos = 0;
    int num_actual = rango;

    while(num_actual > 1)
    {
        //Si el numero es impar
        if((num_actual%2) != 0)
        {
            num_actual = (num_actual*3)+1;
        }
        //Si el numero es par
        else
        {
            num_actual = num_actual/2;
        }
        cant_pasos++;
    }

    //Retorna la cantidad de pasos llevados a cabo para
    //llegar al numero 1.
    return cant_pasos;
}
