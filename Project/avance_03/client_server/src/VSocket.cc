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
 *  Grupos: 2
 *
 **/

#include <cstddef>
#include <stdexcept>
#include <cstdio>
#include <cstring>			// memset

#include <sys/socket.h>
#include <arpa/inet.h>			// ntohs
#include <unistd.h>			// close
//#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>			// getaddrinfo, freeaddrinfo
#include <iostream>
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
   int isIpPv6 = (this->IPv6 == false) ? AF_INET : AF_INET6;
   int socketType = (t == 'd') ? SOCK_DGRAM : SOCK_STREAM;  

   idSocket = socket(isIpPv6, socketType ,0);

  const int enable = 1;

  if (setsockopt(this->idSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
    perror("setsockopt(SO_REUSEADDR) failed");
  }
}


/**
  *  Class initializer
  *     use Unix socket system call
  *
  *  @param     int id: descriptor for an already opened socket (accept)
  *
 **/
void VSocket::InitVSocket( int id ){
   this->idSocket = id;

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
   int st = close(idSocket);

   if ( -1 == st ) {
      throw std::runtime_error( "Socket::Close()" );
   }

}


/**
  * DoConnect method
  *   use "connect" Unix system call
  *
  * @param      char * host: host address in dot notation, example "10.1.104.187"
  * @param      int port: process address, example 80
  *
 **/
int VSocket::DoConnect( const char * hostip, int port ) {
   int st;
   size_t len;
   struct sockaddr_in  host4;
   struct sockaddr_in6  host6;
   struct sockaddr * ha;

   if (IPv6) {
      memset( &host6, 0, sizeof( host6 ) );
      host6.sin6_family = AF_INET6;
      st = inet_pton( AF_INET6, hostip, &host6.sin6_addr );
      if ( -1 == st ) {	// 0 means invalid address, -1 means address error
         throw std::runtime_error( "Socket::Connect( const char *, int ) [inet_pton]" );
      }
      host6.sin6_port = htons( port );
      ha = (struct sockaddr *) &host6;
      len = sizeof( host6 );
      st = connect( this->idSocket, ha, len );
   } else {
      memset( (char *) &host4, 0, sizeof( host4 ) );
      host4.sin_family = AF_INET;
      st = inet_pton( AF_INET, hostip, &host4.sin_addr );
      if ( -1 == st ) {
         throw( std::runtime_error( "VSocket::DoConnect, inet_pton" ));
      }
      host4.sin_port = htons( port );
      st = connect( idSocket, (sockaddr *) &host4, sizeof( host4 ) );
      if ( -1 == st ) {
         throw( std::runtime_error( "VSocket::DoConnect, connect port" ));
      }
   }

   return st;
}


/**
  * DoConnect method
  *   use "connect" Unix system call
  *
  * @param      char * host: host address in dns notation, example "os.ecci.ucr.ac.cr"
  * @param      char * service: process address, example "http"
  *
 **/
int VSocket::DoConnect( const char *host, const char *service ) {
   int st;

   struct addrinfo hints, *result, *rp;
   memset(&hints, 0, sizeof(struct addrinfo));
   hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
   hints.ai_socktype = SOCK_STREAM; /* Stream socket */
   hints.ai_flags = 0;
   hints.ai_protocol = 0;          /* Any protocol */

   st = getaddrinfo( host, service, &hints, &result );

   for ( rp = result; rp; rp = rp->ai_next ) {
      st = connect( idSocket, rp->ai_addr, rp->ai_addrlen );
      if ( 0 == st )
         break;
   }

   freeaddrinfo( result );
   
   if ( -1 == st ) {
      perror( "VSocket::connect" );
      throw std::runtime_error( "VSocket::DoConnect service" );
   }

   return st;

}


/**
  * Listen method
  *
  * @param      int queue: max pending connections to enqueue (server mode)
  *
  *  This method define how many elements can wait in queue
  *
 **/
int VSocket::Listen( int queue ) {
   int st = listen(this->idSocket, queue);
   

   if ( -1 == st ) {
      throw std::runtime_error( "VSocket::Listen( int )" );
   }

   return st;

}

/**
  * Bind method
  *    use "bind" Unix system call (man 3 bind) (server mode)
  *
  * @param      int port: bind a unamed socket to a port defined in sockaddr structure
  *
  *  Links the calling process to a service at port
  *
 **/
int VSocket::Bind( int port ) {
   int st = -1;

   if(this->IPv6) {
      struct sockaddr_in6 host6;
      memset(&host6, 0, sizeof( host6 ) );
      host6.sin6_family = AF_INET6;
      host6.sin6_port = htons(port);
      host6.sin6_addr = in6addr_any;
      st = bind(idSocket, (struct sockaddr *) &host6, sizeof(host6));      
   } else {
      struct sockaddr_in host4;
      memset( (char *) &host4, 0, sizeof( host4 ) );
      host4.sin_family = AF_INET;
      host4.sin_port = htons(port);
      host4.sin_addr.s_addr = INADDR_ANY;
      st = bind(idSocket, (struct sockaddr *) &host4, sizeof(host4));
   }

      
   return st;

}


/**
  * DoAccept method
  *    use "accept" Unix system call (man 3 accept) (server mode)
  *
  *  @returns   a new class instance
  *
  *  Waits for a new connection to service (TCP mode: stream)
  *
 **/
int VSocket::DoAccept(){
   struct sockaddr_in Ahost;
   socklen_t AhostLen = sizeof(struct sockaddr);
   int st = accept(this->idSocket, (struct sockaddr *) &Ahost, &AhostLen);
   
   if ( -1 == st ) {
      perror("VSocket::DoAccept()");
      throw std::runtime_error( "VSocket::DoAccept()" );
   }

   return st;

}


/**
  * Shutdown method
  *    use "shutdown" Unix system call (man 3 shutdown)
  *
  *  @param	int mode define how to cease socket operation
  *
  *  Partial close the connection (TCP mode)
  *
 **/
int VSocket::Shutdown( int mode ) {
   int st = shutdown(this->idSocket, mode);

   if ( -1 == st ) {
      throw std::runtime_error( "VSocket::Shutdown( int )" );
   }

   return st;

}


/**
  *  sendTo method
  *
  *  @param	const void * buffer: data to send
  *  @param	size_t size data size to send
  *  @param	void * addr address to send data
  *
  *  Send data to another network point (addr) without connection (Datagram)
  *
 **/
size_t VSocket::sendTo( const void * buffer, size_t size, void * addr ) {
   const struct sockaddr * host = (const struct sockaddr *) addr;
   socklen_t hostLen = sizeof(*host);

   // std::cout << "after send" << std::endl;
   int st = sendto(idSocket, buffer, size, 0,
                      host, hostLen);

   // std::cout << "before send" << std::endl;
   if(st == -1) {
      throw( std::runtime_error( "VSocket::sendTo, sendto" ));
   }

   return st;
}


/**
  *  recvFrom method
  *
  *  @param	const void * buffer: data to send
  *  @param	size_t size data size to send
  *  @param	void * addr address to receive from data
  *
  *  @return	size_t bytes received
  *
  *  Receive data from another network point (addr) without connection (Datagram)
  *
 **/
size_t VSocket::recvFrom( void * buffer, size_t size, void * addr ) {
   struct sockaddr* host = (struct sockaddr*) addr;
   socklen_t hostLen= sizeof(*host);
   int st = recvfrom(idSocket, buffer, size, 0, host, &hostLen);

   if(st == -1) {
      throw( std::runtime_error( "VSocket::recvFrom, recvfrom" ));
   }

   return st;
}
