
/**
 *  Establece la definición de la clase Socket para efectuar la comunicación
 *  de procesos que no comparten memoria, utilizando un esquema de memoria
 *  distribuida.  El desarrollo de esta clase se hará en varias etapas, primero
 *  los métodos necesarios para los clientes, en la otras etapas los métodos para el servidor,
 *  manejo de IP-v6, conexiones seguras y otros
 *
 *  Universidad de Costa Rica
 *  ECCI
 *  CI0123 Proyecto integrador de redes y sistemas operativos
 *  2023-ii
 *  Grupos: 2 y 3
 *
 * (versión Fedora)
 *
 **/

#include <cstddef>
#include <stdexcept>
#include <cstdio>
#include <cstring>			// memset

#include <sys/socket.h>
#include <arpa/inet.h>		// ntohs
#include <unistd.h>			// close
//#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>			// getaddrinfo, freeaddrinfo

#include "VSocket.h"


/**
  *  Class initializer
  *     use Unix socket system call
  *
  *  @param     char t: socket type to define
  *     's' for stream
  *     'd' for datagram
  *  @param     bool ipv6: if we need a IPv6 socket
  *
 **/
void VSocket::InitVSocket( char t, bool IPv6 ){
  this->IPv6 = IPv6;
  if (IPv6) {
    if ( t == 's' ) {
      this->idSocket = socket(AF_INET6, SOCK_STREAM, 0);
    } else if ( t == 'd' ) {
      this->idSocket = socket(AF_INET6, SOCK_DGRAM, 0);
    }
  } else {
    if ( t == 's' ) {
      this->idSocket = socket(AF_INET, SOCK_STREAM, 0);
    } else if ( t == 'd' ) {
      this->idSocket = socket(AF_INET, SOCK_DGRAM, 0);
    }
  }
}


/**
  * Class destructor
  *
 **/
VSocket::~VSocket() {

  this->Close();

}


/**
  * Close method
  *    use Unix close system call (once opened a socket is managed like a file in Unix)
  *
 **/
void VSocket::Close(){

  int st;
  
  st = close(this->idSocket);

  if ( -1 == st ) {
    throw std::runtime_error( "Socket::Close()" );
  }

}


/**
  * Connect method
  *   use "connect" Unix system call
  *
  * @param      char * host: host address in dot notation, example "10.1.104.187"
  * @param      int port: process address, example 80
  *
 **/
int VSocket::DoConnect( const char * hostip, int port ) {
  int st;
  if ( this->IPv6 ) {
    struct sockaddr_in6  host6;
    struct sockaddr * ha;

    memset( (char *) &host6, 0, sizeof( host6 ) );
    host6.sin6_family = AF_INET6;
    host6.sin6_port = htons( port );
    st = inet_pton( AF_INET, hostip, &host6.sin6_addr );
    if ( 0 <= st ) {
      throw( std::runtime_error( "VSocket::DoConnect, inet_pton" ) );
    }
    ha = (struct sockaddr *) &host6;
    size_t len = sizeof( host6 );
    st = connect( this->idSocket, ha, len );
    if ( -1 == st ) {
      throw std::runtime_error( "Socket::Connect( const char *, int ) [connect]" );
    }

  } else {
    struct sockaddr_in  host4;
    memset( (char *) &host4, 0, sizeof( host4 ) );
    host4.sin_family = AF_INET;
    st = inet_pton( AF_INET, hostip, &host4.sin_addr );
    if ( -1 == st ) {
      throw( std::runtime_error( "VSocket::DoConnect, inet_pton" ) );
    }
    host4.sin_port = htons( port );
    st = connect( idSocket, (sockaddr *) &host4, sizeof( host4 ) );
    if ( -1 == st ) {
      throw( std::runtime_error( "VSocket::DoConnect, connect" ) );
    }
  }

  return st;

}

int VSocket::DoConnect( const char * hostip, const char * service ) {
  int st;
  struct addrinfo hints, *result, *rp;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET6;    /* Allow IPv4 or IPv6 */
  hints.ai_socktype = SOCK_STREAM; /*Dgram socket */
  hints.ai_flags = 0;
  hints.ai_protocol = 0;          /* Any protocol */

  st = getaddrinfo( hostip, service, &hints, &result );

  for ( rp = result; rp; rp = rp->ai_next ) {
    st = connect( idSocket, rp->ai_addr, rp->ai_addrlen );
    if ( 0 == st )
      break;
  }

  freeaddrinfo( result );
  return st;
}
