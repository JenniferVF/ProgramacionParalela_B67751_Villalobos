#include "Numeros.h"
#include <vector>
#include <string.h>
#include <iostream>

Numeros::Numeros()
{
}

Numeros::~Numeros()
{
}

/*
*Metodo encargado de encontrar los numeros pares
*y hacer el calculo respectivo con cada uno de ellos.
*/
std::vector<std::vector<int>> Numeros::Pares(int inicio, int fin, std::vector<int> primos)
{
    std::vector<int> pares; //Vector que almacena todos los numeros pares del rango ingresado.
    std::vector<int> calculo;  //Vector que almacena los digitos de suma de un numero dentro del rango.
    std::vector<std::vector<int>> todos;  //Almacena todas las sumas de todos los numeros pares.
    int start = inicio; //Variable que toma el valor inicial del rango

    //Si el rango del hilo inicia en 1, se toman los numeros desde 5.
    if(inicio == 1)
    {
        start = 5;
    }

    for(int number=start; number <= fin; number++)
    {
        //Si el numero es par, se agrega al vector
        if(number % 2 == 0)
        {
            pares.push_back(number);
        }
    }

    for(size_t i = 0; i < pares.size(); i++)
    {
        calculo = Suma(pares[i], primos);
        todos.push_back(calculo);
    }

    return todos;
}


/*
*Metodo encargado de encontrar todos los numeros
*primos del numero entrante.
*/
std::vector<int> Numeros::Create(int top)
{
    std::vector<int> primos;
    int divisores;
    int divisor;

    //Se guardan todos los numeros primos
    for (int num=2; num <= top; num++)
    {
        divisores = 0;
        divisor = 1;

        do
        {
            if(num % divisor == 0)
            {
                divisores++;
            }
            divisor++;
        }
        while(divisor <= num);

        //El numero es impar: solo se divide entre 1 y si mismo.
        if(divisores == 2)
        {
            primos.push_back(num);
        }
    }

    return primos;
}


/*
*Metodo encargado de encontrar los dos digitos que al
*sumarlos, da como resultado el numero ingresado.
*/
std::vector<int> Numeros::Suma(int par, std::vector<int> primo)
{
    int resultado;
    int termina = 1;
    int first;
    int second;
    std::vector<int> foundit (3, 0);
    int inicio;

    //Para variar los resultados, de 14 en adelante, no se toma el 3.
    if(par < 14)
    {
        inicio = 0;
    }
    else
    {
        inicio = 1;
    }

    for(size_t primero = inicio; primo[primero] < par; primero++)
    {
        if(termina == 1) //Si no se ha encontrado un resultado
        {
            for(size_t segundo = 0; primo[segundo] < par; segundo++)
            {
                if(termina == 1) //Si no se ha encontrado un resultado
                {
                    resultado = primo[primero]+primo[segundo];
                    if(resultado == par)
                    {
                        first = primo[primero];
                        second = primo[segundo];
                        termina = 0;
                    }
                }
            }
        }
    }

    foundit.at(0) = par;
    foundit.at(1) = first;
    foundit.at(2) = second;
    return foundit;
}
