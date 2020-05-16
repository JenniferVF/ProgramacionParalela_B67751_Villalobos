#ifndef ASCENSOR_H_INCLUDED
#define ASCENSOR_H_INCLUDED

/*
 *
 *  Esta clase representa al ascensor que da servicio a las personas
 *  El ascensor tiene reglas de comportamiento establecidas en el enunciado
 *     - No cambiar de sentido hasta estar vacio
 *     - Capacidad limitada: 10 personas
 *
 *  Author: Programacion Paralela y Concurrente
 *  Date: 2020/Abr/23
 *
 */

#include <vector>

class Ascensor
{
public:
    Ascensor( long );
    ~Ascensor();
    int isFull();
    int recorrido(std::vector<std::vector<int>>, std::vector<std::vector<int>>, char*);
    int recorridoArriba(int, int, char*);
    int recorridoAbajo(int, int, char*);
    int solicitud(std::vector<std::vector<int>>, char*);
    void msgBaja(std::vector<int>, char*);
    void msgSube(std::vector<int>, char*);
    void Display( char * );

private:
    long idAscensor;
    int capacidad;
    bool direccion;
    unsigned int pisoActual;

};


#endif // ASCENSOR_H_INCLUDED
