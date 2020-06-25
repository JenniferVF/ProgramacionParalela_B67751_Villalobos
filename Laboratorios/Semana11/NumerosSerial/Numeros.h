#ifndef NUMEROS_H_INCLUDED
#define NUMEROS_H_INCLUDED

#include <vector>
#include <string.h>
#include <iostream>

class Numeros
{

public:
    Numeros();
    ~Numeros();
    void Pares(int);
    std::vector<int> Primos(int);
    std::vector<int> Suma(int, std::vector<int>);
    void imprimir(std::vector<std::vector<int>>);
};

#endif // NUMEROS_H_INCLUDED
