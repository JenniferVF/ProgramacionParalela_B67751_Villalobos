#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bits/stdc++.h>
#include <math.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

using namespace std;

int Collatz(int);
bool Potencia(int);
int Procesos(int);

int  main(int  argc, char *argv[])
{
    int limite; //Numero elegido por el usuario
    int cant_pasos; //Cantidad e pasos en que se realizo la conjetura de Collatz.
    int master_id; //El id del proceso padre
    int son_id; //El id del (de los) proceso(s) hijo(s)
    int cant_procs; //Cantidad de procesos por crear
    int rango; //El rango que le corresponde a cada proceso hijo
    int start; //Inicio del rango
    int top; //Fin del rango

    int    memory_id;
    int    *shared_ptr;
    pid_t  pid;
    int    status;

    //Se le pide al usuario que ingrese un numero.
    if (argc != 2)
    {
        printf("Ingrese: %s <cantidad limite> (que sea potencia de 10)\n", argv[0]);
        exit(1);
    }

    //Si el numero ingresado no es potencia de 10.
    if(!Potencia(atoi(argv[1])))
    {
        printf("El numero ingresado debe ser potencia de 10.\nIngrese: %s <cantidad limite> (que sea potencia de 10)\n", argv[0]);
        exit(1);
    }

    //Se define el rango: 2, limite.
    limite = atoi(argv[1]);
    cant_procs = Procesos(limite);
    rango = limite/cant_procs;
    //Se establece en cero los elementos de la memoria compartida
    shared_ptr[0] = 0;
    shared_ptr[1] = 0;

    //Se crea el segmento de memoria compartida
    memory_id = shmget(IPC_PRIVATE, 2*sizeof(int), IPC_CREAT | 0666);
    if (memory_id < 0)
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
    shared_ptr = (int *) shmat(memory_id, NULL, 0);
    if ((intptr_t) shared_ptr == -1)
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
                if(cant_pasos > shared_ptr[1])
                {
                    shared_ptr[0] = i;
                    shared_ptr[1] = cant_pasos;
                }
            }
        }
        //Si son los demas procesos, el inicio se toma normal.
        else
        {

            for(int i = start; i <= top; i++)
            {
                cant_pasos = Collatz(i);
                if(cant_pasos > shared_ptr[1])
                {
                    shared_ptr[0] = i;
                    shared_ptr[1] = cant_pasos;
                }
            }
        }
    }

    //Cada hijo se separa de la memoria compartida.
    if(shmdt((void *) shared_ptr) == -1)
    {
        perror("Error: shmdt\n");
    }


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
    shared_ptr = (int*) shmat(memory_id, NULL, 0);
    //Se muestra el resultado
    printf("Limite superior indicado: %d \nElemento del conjunto con mas pasos: %d \nPasos realizados: %d \n\n",
           limite, shared_ptr[0], shared_ptr[1]);

    //El proceso master elimina la memoria compartida.
    shmctl(memory_id, IPC_RMID, NULL);
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

/*
*Metodo encargado de calcular los hijos/procesos
*necesarios, segun el limite superior ingresado
*por el usuario.
*/
int Procesos(int limite)
{
    int cant_procs;
    int potencia;

//Se obtiene la potencia de 10 correspondiente
//del numero ingresado.
    potencia = log10(limite);

    //Si el numero es 10, 100 o 1000: 2 hijos.
    if(potencia <= 3)
    {
        cant_procs = 2;
    }
    //Si el numero va desde 10000 a un millon: 5 hijos.
    else if(potencia > 3 && potencia < 7)
    {
        cant_procs = 5;
    }
    //Si el numero es 10 millones o 100 millones: 10 hijos.
    else if(potencia > 7 && potencia < 9)
    {
        cant_procs = 10;
    }
    //Si el numero es mayor: 20 hijos.
    else
    {
        cant_procs = 20;
    }

    return cant_procs;
}


/*
*Metodo que revisa si el numero ingresado es una potencia de 10.
*/
bool Potencia(int numero)
{
    int potencia = log10(numero);

    //Si el resultado elevado a la potencia
    //es igual al numero entrante, este es
    //potencia de 10.
    if(pow(10, potencia) == numero)
    {
        return true;
    }
    else
    {
        return false;
    }
}
