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
    std::vector<std::vector<int>> Pares(int, int, std::vector<int>);
    std::vector<int> Create(int);
    std::vector<int> Suma(int, std::vector<int>);
    void imprimir(std::vector<std::vector<int>>);
};

#endif // NUMEROS_H_INCLUDED
