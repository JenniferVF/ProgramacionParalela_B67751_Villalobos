/*
 *
 *  Esta clase representa al ascensor que da servicio a las personas
 *  Las instancias inicial en el primer piso
 *
 *  Author: Programacion Paralela y Concurrente
 *  Date: 2020/Abr/23
 *
 */

#include <stdio.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "Ascensor.h"

/*
 *
 */
Ascensor::Ascensor( long identificador )
{

    this->idAscensor = identificador;
    this->pisoActual = 1;
    this->capacidad = 0;
    this->direccion = true; //True: sube. False: baja.

}

Ascensor::~Ascensor()
{
}

/*
*Metodo que avisa si el ascensor se
*encuentra lleno, vacio, o posee carga
*pero no se encuentra en su limite.
*/
int Ascensor::isFull()
{
    //Si se encuentra lleno, retorne un 1
    if(this->capacidad == 8)
    {
        return 1;
    }
    //Si esta vacio, retorna 0
    else if (this->capacidad == 0)
    {
        return 0;
    }
    //Si tiene carga pero no llega a su limite, retorna -1
    else
    {
        return -1;
    }
}

void Ascensor::solicitud(std::vector<int> person_id, char* rotulo)
{
    int lleno;
    lleno = this->isFull();

    if(lleno == -1 || lleno == 0)
    {
        inside.push_back(person_id);
        this->capacidad++;
        sprintf( rotulo, "Persona[ %d ]: --Solicitud plantada--\n", person_id[0] );
    }
    else
    {
        cola.push_back(person_id);
        sprintf( rotulo, "Ascensor[ %d ]: Capacidad superada. Persona [%d] debe esperar a que alguien se baje.\n", this->idAscensor, person_id[0] );
    }

}

void Ascensor::Orden()
{
    //Se ordena el vector de las personas que van dentro del ascensor, segun el piso en el que bajan.
    std::sort(inside.begin(), inside.end(), [](const std::vector<int>& a, const std::vector<int>&b)
    {
        return a[2] < b[2];
    });

    //Se ordena el vector de las personas que aun no han subido, segun el piso en el que suben.
    std::sort(cola.begin(), cola.end(), [](const std::vector<int>& a, const std::vector<int>&b)
    {
        return a[1] < b[1];
    });

}

std::vector<int> Ascensor::recorridoInside()
{
    std::vector<int> actual;
    std::vector<int> baja;
    int piso;

    //Primero se recorre el vector de las personas que se
    //encuentran dentro del ascensor y necesitan bajar.
    for(std::size_t i = 0; i < inside.size(); i++)
    {
        actual = inside[i];
        piso = actual[2];

        //Si el ascensor va para arriba
        if(this->direccion == true)
        {
            if(piso >= this->pisoActual)
            {
                baja = actual;
                break;
            }
        }
        //Si el ascensor va para abajo
        else
        {
            if(piso <= this->pisoActual)
            {
                baja = actual;
                break;
            }
        }

    }

    return baja;
}

std::vector<int> Ascensor::recorridoCola()
{
    std::vector<int> actual;
    std::vector<int> sube;
    int piso;

    //Primero se recorre el vector de las personas que se
    //encuentran dentro del ascensor y necesitan bajar.
    for(std::size_t i = 0; i < cola.size(); i++)
    {
        actual = cola[i];
        piso = actual[1];

        //Si el ascensor va para arriba
        if(this->direccion == true)
        {
            if(piso > this->pisoActual)
            {
                sube = actual;
                break;
            }
        }
        //Si el ascensor va para abajo
        else
        {
            if(piso < this->pisoActual)
            {
                sube = actual;
                break;
            }
        }

    }

    return sube;
}


void Ascensor::upOrDown(char * rotulo)
{
    std::vector<int> baja;
    std::vector<int> sube;

    Orden(); //Se ordenan los vectores de menor a mayor
    baja = recorridoInside();
    sube = recorridoCola();

    //Se baja a la persona.
    if(this->pisoActual == baja[2])
    {
        sprintf( rotulo, "Ascensor[ %d ]: Piso actual %d. --Bajan--\n", this->idAscensor, this->pisoActual );
        sprintf( rotulo, "Persona[ %d ]: Bajando en piso %d.\n", baja[0], baja[2] );
        //Se elimina esa persona del vector inside
        inside.erase(std::find(inside.begin(), inside.end(), baja[0]));
        //Se disminuye la capacidad
        this->capacidad--;
        //Se pasa alguien de la cola de espera a inside
        inside.push_back(cola.back());
        cola.pop_back();
    }
    else
    {
        //Si el ascensor va para arriba
        if(this->direccion == true)
        {
            if(baja[2] > sube[1])
            {
                sprintf( rotulo, "Ascensor[ %d ]: Bajan en el piso %d, pero suben en el piso %d. Vamos para el piso %d.\n", baja[2], sube[1], sube[1]);
                this->pisoActual=sube[1];
            }
            else
            {
                sprintf( rotulo, "Ascensor[ %d ]: Suben en el piso %d, pero bajan en el piso %d. Vamos para el piso %d.\n", sube[1], baja[2], baja[2]);
                this->pisoActual=baja[2];
            }
        }
        //Si el ascensor va para abajo
        else
        {
            if(baja[2] > sube[1])
            {
                sprintf( rotulo, "Ascensor[ %d ]: Suben en el piso %d, pero bajan en el piso %d. Vamos para el piso %d.\n", sube[1], baja[2], baja[2]);
                this->pisoActual=baja[2];
                sprintf( rotulo, "Persona[ %d ]: Baja en el piso %d, pero bajan en el piso %d. Vamos para el piso %d.\n", sube[1], baja[2], baja[2]);
            }
            else
            {
                sprintf( rotulo, "Ascensor[ %d ]: Bajan en el piso %d, pero suben en el piso %d. Vamos para el piso %d.\n", baja[2], sube[1], sube[1]);
                this->pisoActual=sube[1];
            }
        }
    }
}

void Ascensor::Display( char * rotulo )
{

    sprintf( rotulo, "Ascensor[ %d ]: Piso actual %d.\n", this->idAscensor, this->pisoActual );

}

int Ascensor::getCapacidad()
{
    return this->capacidad;
}
