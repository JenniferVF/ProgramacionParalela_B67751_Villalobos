#include <iostream>
#include <math.h>
#include <vector>
#include <omp.h>
#include <time.h>
#include <chrono>
#include "Numeros.h"

using namespace std;

int main(int  argc, char *argv[])
{
    Numeros * num;
    num = new Numeros();

    int limite = 0; //Almacena el limite ingresado por el usuario
    int cant_hilos = 0;
    int rango = 0;
    vector<vector<int>> ready; //Vector que almacena todas las sumas de todos los numeros.
    vector<vector<int>> oneMore; //Vector que almacena todas las sumas realizadas en el rango correspondiente.
    vector<int> actual; //Vector que almacena solo una suma y la agrega al vector global.
    vector<int> primos;  //Vector que almacena todos los numeros primos del limite ingresado.
    int range = 0;  //Almacena el rango de cada hilo
    int inicio = 0;  //Inicio del rango de cada hilo
    int fin = 0;  //Fin del rango de cada hilo
    int id = 0;  //Identificacion del hilo
    double tiempo;

    auto start = chrono::high_resolution_clock::now();

    //Se le pide al usuario que ingrese un numero (el limite).
    if (argc != 3)
    {
        printf("Ingrese: %s <cantidad limite> y <numero de hilos>. \n", argv[0]);
        exit(1);
    }

    limite = atoi(argv[1]);
    cant_hilos = atoi(argv[2]);

    //En caso de que la cantidad de numeros no sea divisible entre la cantidad de hilos.
    rango = floor(limite/cant_hilos);
    range = (limite - rango*(cant_hilos-1));

    //Antes de la parte paralela, se obtienen los numeros primos
    primos = num->Create(limite);

    #pragma omp parallel num_threads(cant_hilos) shared (ready, primos) private (oneMore, actual, inicio, fin)
    {
        id = omp_get_thread_num();

        //Se crea el rango de cada hilo segun su id.
        inicio = (range * id) + 1;
        fin = range * (id+1);

        //Si el final del rango es mayor que el limite ingresado por el usuario.
        if (fin > limite)
        {
            fin = limite;
        }

        oneMore = num->Pares(inicio, fin, primos);

        for(size_t i = 0; i < oneMore.size(); i++)
        {
            actual = oneMore[i];
            //Solo un hilo a la vez puede agregar la suma al vector global
            #pragma omp critical
            ready.push_back(actual);
        }
    }

    //Solo un hilo se encarga de imprimir los resultados
    #pragma omp single
    //Imprime el resultado
    for(size_t num = 0; num < ready.size(); num++)
    {
        actual = ready[num];
        printf( "%d = %d + %d. \n",actual[0], actual[1], actual[2]);
    }

    auto finish = chrono::high_resolution_clock::now();

    tiempo = chrono::duration<double, milli>(finish-start).count();
    printf("Tiempo transcurrido en milisegundos: %f. \n", tiempo);
}
