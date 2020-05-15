/*
 *
 *  Esta clase representa a las personas que utilizan el ascensor
 *  Cada persona es creada con los pisos de subida y bajada definidos
 *  Las personas deben esperar hasta que el ascensor llegue a mi piso de subida,
 *  comprobar que haya campo y montarse al ascensor hasta su piso de bajada
 *
 *  Author: Programacion Paralela y Concurrente
 *  Date: 2020/Abr/23
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#include "Persona.h"


/*
 *  Contruye una persona usuaria del ascensor
 *  Debe definir de manera aleatoria los pisos de subida y bajada
 *  Los pisos deben respetar las limitaciones fisicas del edificio
 *     - Se indica 10 pisos en el enunciado numerados del 1 al 10
 *
 */
Persona::Persona( long identificacion )
{
    int r1, r2;

    this->idPersona = identificacion;

    r1 = (rand() % 10) + 1;
    this->pisoDondeMeSubo = r1;

    do
    {
        r2 = (rand() % 10) + 1;
    }
    while ( r1 == r2 );
    this->pisoDondeMeBajo = r2;

}


/*
 *  Destructor
 */
Persona::~Persona()
{
}

std::vector<int> Persona::llamado( char * rotulo)
{
    std::vector<int> identidad;
    identidad.push_back(this->idPersona);
    identidad.push_back(this->pisoDondeMeSubo);
    identidad.push_back(this->pisoDondeMeBajo);

    sprintf( rotulo, "Persona[ %d ]: Esperando para plantar solicitud\n", this->idPersona );

    return identidad;
}

/*
 *  Metodo para desplegar los datos de los elementos de esta clase (toString)
 */
void Persona::Display( char * rotulo )
{

    sprintf( rotulo, "Persona[ %d ]: Subo en %d, bajo en %d\n", this->idPersona, this->pisoDondeMeSubo, this->pisoDondeMeBajo );

}
