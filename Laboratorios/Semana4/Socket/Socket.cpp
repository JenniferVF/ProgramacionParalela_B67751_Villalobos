/*
 *  Esqueleto para la clase Socket
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Socket.h"


/*
   char tipo: el tipo de socket que quiere definir
      's' para "stream
      'd' para "datagram"
   bool ipv6: si queremos un socket para IPv6
 */
Socket::Socket( char tipo, bool ipv6 )
{
    //Si el bool es true
    if(ipv6)
    {
        if(tipo == 'd')
        {
            this->idSocket = socket(AF_INET6, SOCK_DGRAM, 0);
        }
        else
        {
            this->idSocket = socket(AF_INET6, SOCK_STREAM, 0);
        }
    }
    else     //conexion ipv4
    {
        if(tipo == 'd')
        {
            this->idSocket = socket(AF_INET, SOCK_DGRAM, 0);
        }
        else
        {
            this->idSocket = socket(AF_INET, SOCK_STREAM, 0);
        }
    }
    //En caso de error
    if (idSocket == -1)
    {
        perror("Socket::Socket");
        exit(1);
    }
}

/*
Se encarga de crear un nuevo socket segun el nuevo
id enviado por el metodo bind.
*/
Socket::Socket( int newId )
{
    // Crea un socket de IPv4, tipo "stream"
    this->idSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (idSocket == -1)
    {
        perror("Socket::Socket");
        exit(1);
    }

    //Se establece la nuevo id
    SetIDSocket(newId);
}


// Crea un socket de IPv4, tipo "stream"
Socket::Socket()
{
    this->idSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (idSocket == -1)
    {
        perror("Socket::Socket");
        exit(1);
    }

}


Socket::~Socket()
{
    Close();
}


void Socket::Close()
{
    int apagado = close(this->idSocket);
    //En caso de error
    if (apagado != 0)
    {
        perror("Socket::Close");
        exit(1);
    }

}

/*
   char * hostip: direccion del servidor, por ejemplo "www.ecci.ucr.ac.cr"
   int port: ubicacion del proceso, por ejemplo 80
 */
int Socket::Connect( char * hostip, int port )
{
    //Para definir los elementos del socket
    struct sockaddr_in direccion;
    direccion.sin_family = AF_INET;
    direccion.sin_port = htons(port); //Se convierte a variable de red

    inet_aton(hostip, &direccion.sin_addr);

    int conexion = connect(this->idSocket, (struct sockaddr *) &direccion, sizeof(direccion));

     if(conexion == -1)
        {
            perror("Socket::Connect");
            exit(1);
        }
    return conexion;

}


/*
   char * hostip: direccion del servidor, por ejemplo "www.ecci.ucr.ac.cr"
   char * service: nombre del servicio que queremos acceder, por ejemplo "http"
 */
int Socket::Connect( char *host, char *service )
{
    int conexion;
    struct addrinfo hints, *resultado, *rp;

    memset(&hints, 0, sizeof(struct addrinfo));

    //Criterios de la direccion del socket
    hints.ai_family = AF_UNSPEC;  //Permite IPv4 o IPv6.
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0; //Cualquier protocolo

    //Resultado guarda la direccion a internet
    conexion = getaddrinfo(host, service, &hints, &resultado);

    for(rp = resultado; rp; rp = rp->ai_next)
    {
        conexion = connect(idSocket, rp->ai_addr, rp->ai_addrlen);
        if(conexion == -1)
        {
            perror("Socket::Connect");
            exit(1);
        }

    }

    //Se libera la direccion guardada
    freeaddrinfo(resultado);
    return conexion;
}


int Socket::Read( char *text, int len )
{
    int lectura = read(this->idSocket, (void *)text, len);
    //En caso de error
    if (lectura == -1)
    {
        perror("Socket::Read");
        exit(1);
    }
    return lectura;
}


int Socket::Write( char *text, int len)
{
    int escritura = write(this->idSocket, (void *)text, len);
    if (escritura == -1)
    {
        perror("Socket::Write");
        exit(1);
    }
    return escritura;
}


int Socket::Write( char *text)
{
    int escritura = write(this->idSocket, (void *)text, strlen(text));
    if (escritura == -1)
    {
        perror("Socket::Write");
        exit(1);
    }
    return escritura;
}


int Socket::Listen( int queue )
{
    int spasivo = listen(this->idSocket, queue);

    if (spasivo == -1)
    {
        perror("Socket::Listen");
        exit(1);
    }
    return spasivo;
}


int Socket::Bind( int port )
{
    //Se detallan las cualidades del socket
    struct sockaddr_in direccion;
    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = htonl(INADDR_ANY);
    direccion.sin_port = htons(port);

    //Se genera la conexion
    int identificacion = bind(this->idSocket, (const sockaddr*)&direccion, sizeof(direccion));
    if(identificacion == -1)
    {
        perror("Socket::Bind");
        exit(1);
    }
    return identificacion;
}


Socket * Socket::Accept()
{
    struct sockaddr_in direccion;
    socklen_t direccion_len = sizeof(direccion);

    //Se crea una nueva direccion
    int newId = accept(this->idSocket, (struct sockaddr*) &direccion, &direccion_len);

    if(newId == -1)
    {
        perror("Socket::Accept");
        exit(1);
    }

    //Se crea un nuevo Socket con la direccion obtenida
    Socket * newSocket = new Socket(newId);

    return newSocket;

}


int Socket::Shutdown( int mode )
{
    int apagado = shutdown(this->idSocket, mode);

     if(apagado == -1)
    {
        perror("Socket::Shutdown");
        exit(1);
    }
    return apagado;

}

//Para declarar la nueva id del socket
void Socket::SetIDSocket(int id)
{

    idSocket = id;

}
