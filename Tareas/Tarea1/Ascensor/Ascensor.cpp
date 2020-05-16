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

int Ascensor::solicitud(std::vector<std::vector<int>> cola, char* rotulo)
{
    std::vector<int> person_id = cola.back();
    int entra;
    int lleno;

    lleno = this->isFull();

    if(lleno == -1 || lleno == 0)
    {
        //inside.push_back(person_id);
        //this->capacidad++;
        sprintf( rotulo, "Persona[ %d ]: --Solicitud plantada--\n", person_id[0] );
        entra = 0;
        this->capacidad++;
    }
    else
    {
        //cola.push_back(person_id);
        sprintf( rotulo, "Ascensor[ %d ]: Capacidad superada. Persona [%d] debe esperar a que alguien se baje.\n", this->idAscensor, person_id[0] );
        entra = 1;
    }

    return entra;
}

//void Ascensor::Subir(std::vector<int> person_id){
//
//}

int Ascensor::recorrido(std::vector<std::vector<int>> inside, std::vector<std::vector<int>> bajan, char* rotulo)
{
    std::vector<int> sube;
    std::vector<int> baja;
    int pisoBaja;
    int pisoSube;
    int resultado;

    if(bajan.empty() && inside.empty())
    {
        return 2;
    }

    if(!bajan.empty())
    {
        baja = bajan.back();
        pisoBaja = baja[2];
    }
    else
    {
        return 1;
    }

    if(!inside.empty())
    {
        sube = inside.back();
        pisoSube = sube[1];
    }
    else
    {
        return 0;
    }

    //Si el ascensor va para arriba
    if(this->direccion)
    {
        resultado = recorridoArriba(pisoBaja, pisoSube, rotulo);
    }
    else
    {
        resultado = recorridoAbajo(pisoBaja, pisoSube, rotulo);
    }

    return resultado;
}


int Ascensor::recorridoArriba(int pisoBaja, int pisoSube, char * rotulo)
{
//Si los dos son mayores que el piso actual,
    //el ascensor sube al mas cercano
    if(pisoSube > this->pisoActual && pisoBaja > this->pisoActual)
    {
        if(pisoSube > pisoBaja)
        {
            //El ascensor sube a pisoBaja
            sprintf( rotulo, "Ascensor[ %d ]: Voy para arriba. Suben en el piso %d. Pero bajan en el piso %d. Vamos para el piso %d. \n", this->idAscensor, pisoSube, pisoBaja, pisoBaja);
            return 0;
        }
        else
        {
            sprintf( rotulo, "Ascensor[ %d ]: Voy para arriba. Bajan en el piso %d. Pero suben en el piso %d. Vamos para el piso %d. \n", this->idAscensor, pisoBaja, pisoSube, pisoSube);
            return 1;
        }
    }

    //El ascensor sube a pisoSube
    else if(pisoSube > this->pisoActual && this->pisoActual > pisoBaja)
    {
        sprintf( rotulo, "Ascensor[ %d ]: Voy para arriba. Bajan en el piso %d. Pero suben en el piso %d. Vamos para el piso %d. \n", this->idAscensor, pisoBaja, pisoSube, pisoSube);
        return 1;
    }
    //El ascensor sube a pisoBaja
    else if(pisoBaja > this->pisoActual && this->pisoActual > pisoSube)
    {
        sprintf( rotulo, "Ascensor[ %d ]: Voy para arriba. Suben en el piso %d. Pero bajan en el piso %d. Vamos para el piso %d. \n", this->idAscensor, pisoSube, pisoBaja, pisoBaja);
        return 0;
    }
    //Ambos estan abajo, cambia de direccion y se va al mas cercano
    else
    {
        if(pisoBaja > pisoSube)
        {
            //El ascensor baja a pisoBaja
            sprintf( rotulo, "Ascensor[ %d ]: Ambas llamadas estan en niveles inferiores, se cambia de direccion. Suben en el piso %d. Pero bajan en el piso %d. Vamos para el piso %d. \n", this->idAscensor, pisoSube, pisoBaja, pisoBaja);
            direccion = false;
            return 0;
        }
        else
        {
            sprintf( rotulo, "Ascensor[ %d ]: Ambas llamadas estan en niveles inferiores, se cambia de direccion. Bajan en el piso %d. Pero suben en el piso %d. Vamos para el piso %d. \n", this->idAscensor, pisoBaja, pisoSube, pisoSube);
            direccion = false;
            return 1;
        }
    }
}


int Ascensor::recorridoAbajo(int pisoBaja, int pisoSube, char* rotulo)
{
//Si los dos son menores que el piso actual,
    //el ascensor baja al mas cercano
    if(pisoSube < this->pisoActual && pisoBaja < this->pisoActual)
    {
        if(pisoSube < pisoBaja)
        {
            //El ascensor baja a pisoBaja
            sprintf( rotulo, "Ascensor[ %d ]: Voy para abajo.  Suben en el piso %d. Pero bajan en el piso %d. Vamos para el piso %d. \n", this->idAscensor, pisoSube, pisoBaja, pisoBaja);
            return 0;
        }
        else
        {
            sprintf( rotulo, "Ascensor[ %d ]: Voy para abajo. Bajan en el piso %d. Pero suben en el piso %d. Vamos para el piso %d. \n", this->idAscensor, pisoBaja, pisoSube, pisoSube);
            return 1;
        }
    }

    //El ascensor baja a pisoSube
    else if(pisoSube < this->pisoActual && this->pisoActual < pisoBaja)
    {
        sprintf( rotulo, "Ascensor[ %d ]: Voy para abajo. Bajan en el piso %d. Pero suben en el piso %d. Vamos para el piso %d. \n", this->idAscensor, pisoBaja, pisoSube, pisoSube);
        return 1;
    }
    //El ascensor baja a pisoBaja
    else if(pisoBaja < this->pisoActual && this->pisoActual < pisoSube)
    {
        sprintf( rotulo, "Ascensor[ %d ]: Voy para abajo. Suben en el piso %d. Pero bajan en el piso %d. Vamos para el piso %d. \n", this->idAscensor, pisoSube, pisoBaja, pisoBaja);
        return 0;
    }
    //Ambos estan arriba, cambia de direccion y se va al mas cercano
    else
    {
        if(pisoBaja < pisoSube)
        {
            //El ascensor sube a pisoBaja
            sprintf( rotulo, "Ascensor[ %d ]: Ambas llamadas estan en niveles superiores, se cambia de direccion. Suben en el piso %d. Pero bajan en el piso %d. Vamos para el piso %d. \n", this->idAscensor, pisoSube, pisoBaja, pisoBaja);
            direccion = true;
            return 0;
        }
        else
        {
            sprintf( rotulo, "Ascensor[ %d ]: Ambas llamadas estan en niveles superiores, se cambia de direccion. Bajan en el piso %d. Pero suben en el piso %d. Vamos para el piso %d. \n", this->idAscensor, pisoBaja, pisoSube, pisoSube);
            direccion = true;
            return 1;
        }
    }
}


void Ascensor::msgBaja(std::vector<int> id, char *rotulo)
{
    this->pisoActual = id[2];
    sprintf( rotulo, "Persona[ %d ]: --Baja en piso %d-- \n", id[0], id[2]);
    this->capacidad--;
}

void Ascensor::msgSube(std::vector<int> id,  char *rotulo)
{
    this->pisoActual = id[1];
    sprintf( rotulo, "Persona[ %d ]: --Sube en piso %d-- \n", id[0], id[1]);
}

void Ascensor::Display( char * rotulo )
{

    sprintf( rotulo, "Ascensor[ %d ]: Piso actual %d.\n", this->idAscensor, this->pisoActual );

}
