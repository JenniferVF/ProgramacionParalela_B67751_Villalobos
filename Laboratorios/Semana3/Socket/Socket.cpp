/*
 *  Esqueleto para la clase Socket
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
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
    struct sockaddr_in direccion;
    direccion.sin_family = AF_INET;
    direccion.sin_port = htons(port);

    inet_pton(AF_INET, hostip, & direccion.sin_addr);

    int conexion = connect(this->idSocket, (struct sockaddr *) & direccion, sizeof(direccion));
    return conexion;

}


/*
   char * hostip: direccion del servidor, por ejemplo "www.ecci.ucr.ac.cr"
   char * service: nombre del servicio que queremos acceder, por ejemplo "http"
 */
//int Socket::Connect( char *host, char *service )
//{
//    int conexion = connect(idSocket, (sockaddr *) host, sizeof(host));
//    //return -1;
//
//}


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


int Socket::Listen( int queue )
{

    return -1;

}


int Socket::Bind( int port )
{

    return -1;

}


Socket * Socket::Accept()
{

    //return -1;

}


int Socket::Shutdown( int mode )
{

    //return -1;

}


void Socket::SetIDSocket(int id)
{

    idSocket = id;

}
