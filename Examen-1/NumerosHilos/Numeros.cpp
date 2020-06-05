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
*
*/
std::vector<std::vector<int>> Numeros::Pares(int rango, int id)
{
    int start;
    std::vector<int> pares;
    std::vector<int> calculo;
    std::vector<std::vector<int>> todos;
    start = id;

    if(id == 0){
        start = 4;
    }

    for(int number=start; number <= rango; number++)
    {
        //Si el numero es par, se agrega al vector
        if(number % 2 == 0)
        {
            pares.push_back(number);
        }
    }

    for(size_t i = 0; i < pares.size(); i++)
    {
        calculo = Primos(pares[i]);
        todos.push_back(calculo);
    }

    //imprimir(todos);

return todos;
}

/*
*
*/
std::vector<int> Numeros::Primos(int par)
{
    std::vector<int> primo;
    std::vector<int> linea;

    for (int i=2; i<par; i++)
    {
        //El numero es impar
        if(i % 2 != 0)
        {
            primo.push_back(i);
        }
    }

    linea = Suma(par, primo);
    return linea;
}

/*
*
*/
std::vector<int> Numeros::Suma(int par, std::vector<int> primo)
{
    int resultado;
    int first;
    int second;
    std::vector<int> foundit;

    for(size_t primero = 0; primero < primo.size(); primero++)
    {
        for(size_t segundo = 0; segundo < primo.size(); segundo++)
        {
            resultado = primo[primero]+primo[segundo];
            if(resultado == par)
            {
                first = primo[primero];
                second = primo[segundo];
                break;
            }
        }
    }
    foundit.push_back(par);
    foundit.push_back(first);
    foundit.push_back(second);
    return foundit;
}

/*
*
*/
void Numeros::imprimir(std::vector<std::vector<int>> numeros)
{
    std::vector<int> actual;

    for(size_t num = 0; num < numeros.size(); num++)
    {
        actual = numeros[num];
        printf( "%d = %d + %d. \n",actual[0], actual[1], actual[2]);
    }
}