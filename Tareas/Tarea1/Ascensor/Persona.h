#ifndef PERSONA_H_INCLUDED
#define PERSONA_H_INCLUDED

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

#include <vector>

class Persona {
   public:
      Persona( long );
      ~Persona();
      std::vector<int> llamado(char *);
      void Display( char * );

   private:
      long idPersona;
      unsigned int pisoDondeMeSubo, pisoDondeMeBajo;

};

#endif // PERSONA_H_INCLUDED
