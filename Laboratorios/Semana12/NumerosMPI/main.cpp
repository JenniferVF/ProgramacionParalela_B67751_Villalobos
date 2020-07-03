#include <iostream>
#include <math.h>
#include <vector>
#include <mpi.h>
#include <time.h>
#include <chrono>
#include "Numeros.h"

using namespace std;

int main(int  argc, char *argv[])
{
    Numeros * num;
    num = new Numeros();

    vector<vector<int>> total; //Vector que almacena todas las sumas de todos los numeros.
    int limite = 0; //Almacena el limite ingresado por el usuario
    int procesos = 0;
    int cant_procs = 0;
    int rango = 0;
    int range = 0;  //Almacena el rango de cada hilo
    int inicio_local = 0;  //Inicio del rango de cada hilo
    int fin_local = 0;  //Fin del rango de cada hilo
    int id = 0;  //Identificacion del hilo
    double tiempo;
    int my_rank; //Numero del proceso

    //Se inicializa MPI
    MPI_Init(NULL, NULL);

    //Obtiene el numero de procesos
    MPI_Comm_size(MPI_COMM_WORLD, &procesos);

    //Obtiene el numero de proceso
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    auto start = chrono::high_resolution_clock::now();

    //Se le pide al usuario que ingrese un numero (el limite).
    if (argc != 2)
    {
        printf("Ingrese: %s <cantidad limite> y <numero de procesos>. \n", argv[0]);
        exit(1);
    }

    limite = atoi(argv[1]);
    cant_procs = procesos-1;

    //En caso de que la cantidad de numeros no sea divisible entre la cantidad de hilos.
    rango = floor(limite/cant_procs);
    range = (limite - rango*(cant_procs-1));

    if(my_rank != 0)
    {
        vector<vector<int>> local; //Vector que almacena todas las sumas realizadas en el rango correspondiente.
        vector<int> actual; //Vector que almacena solo una suma y la agrega al vector local.
        id = my_rank-1;
        int veces;

        //Se crea el rango de cada hilo segun su id.
        inicio_local = (range * id) + 1;
        fin_local = range * (id+1);

        //Si el final del rango es mayor que el limite ingresado por el usuario.
        if (fin_local > limite)
        {
            fin_local = limite;
        }

        local = num->Pares(inicio_local, fin_local);
        veces = local.size();

        MPI_Send(&veces, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        for(size_t i = 0; i < local.size(); i++)
        {
            actual = local[i];
            MPI_Send(&actual[0], 3, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    else
    {
        vector<int> actual; //Vector que almacena solo una suma y la agrega al vector total.
        int veces = 0;

        for(int proc = 1; proc < procesos; proc++)
        {
            MPI_Recv(&veces, 1, MPI_INT, proc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for(int i = 0; i < veces; i++)
            {
                actual.resize(3); //Crea espacio para 3 ints.
                MPI_Recv(&actual[0], 3, MPI_INT, proc, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                total.push_back(actual);

            }
        }

    }

    //Solo el proceso 0 se encarga de imprimir los resultados
    if(my_rank == 0)
    {
        vector<int> actual; //Vector que almacena solo una suma y la agrega al vector total.

        //Imprime el resultado
        for(size_t num = 0; num < total.size(); num++)
        {
            actual = total[num];
            printf( "%d = %d + %d. \n",actual[0], actual[1], actual[2]);
        }
    }


    if(my_rank == 0)
    {
        auto finish = chrono::high_resolution_clock::now();

        tiempo = chrono::duration<double, milli>(finish-start).count();
        printf("Tiempo transcurrido en milisegundos: %f. \n", tiempo);
    }

    MPI_Finalize();
}
