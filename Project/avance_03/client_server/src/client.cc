/**
 *
 *   UCR-ECCI
 *
 *   IPv4 TCP client normal or SSL according to parameters
 *
 **/

#include <stdio.h>
#include <string.h>

#include "VSocket.h"
#include "Socket.h"
#include <iostream>
#include <string>
#include <regex>
#include <cctype>

#include "json.hpp"

#define	MAXBUF	1024

/**
 * @brief Check if a string contains only digits.
 *
 * @param str The string to check
 * @return true if the string contains only numeric digits, false otherwise
 *
 */
bool isNumber(const std::string& str) {
  for (char c : str) {
      if (!std::isdigit(c)) {
          return false;
      }
  }
  return true;
}
/**
 * @brief Extract matches or coincidences from a string that match a regular expression
 *
 * @param code_html the input string from which matches are extracted.
 * @param exp_regular The regex used to search for matches
 * @param code_html_vector Vector to save the found matches.
 */
void getRegex(std::string code_html, std::string exp_regular, std::vector<std::string>& code_html_vector){
  std::regex pattern(exp_regular);
  std::smatch matches;
  while (std::regex_search(code_html, matches, pattern)) {
      code_html_vector.push_back(matches[1]);
      code_html = matches.suffix().str();
  }
}

bool is_valid(char* options, char* a) {
  std::string figure = a;
  std::string option = options; 
  size_t found = option.find(figure);
  if (found != std::string::npos) {
    return true;
  } else {
    return false;
  }
}

/**
 * @brief Create sockets to get info from server's lego and parse that
 * info using regular expressions.
 */
int main( int argc, char * argv[] ) {
  
  char *hostname, *portnum;

  if ( argc != 3 ) {
    printf("usage: %s <hostname> <portnum>\n", argv[0] );
    exit(0);
  }
  hostname = argv[ 1 ];
  portnum = argv[ 2 ];

  VSocket * client;
  int st, port = 80;
  char a[ MAXBUF ];
  char * os = (char *) "192.168.18.33";
  std::string lego = (char *) "GET /lego/index.php HTTP/1.1\r\nhost: redes.ecci\r\n\r\n"; 

  client = new Socket('s',false);	// Create a new stream socket for IPv4

  memset( a, 0 , MAXBUF );
  client->Connect( hostname, atoi( portnum ) );

  bool flag = false;
  int error;
  const char* terminal = "terminal";
  client->Write(terminal, strlen(terminal));
  client->Read(a, MAXBUF);

  std::cout << "Escriba el nombre de la figura que desea\nOpciones:\n\n" << a 
    << std::endl;

  char option[ MAXBUF ];
  strcpy(option, a);

  while (!flag) {
    memset( a, 0 , MAXBUF );
    std::cin >> a; std::cout << std::endl;
    if (is_valid(option, a)) {
      flag = true;
    } else {
      std::cout << "Invalid option, try again" << std::endl;
    }
  }
// http://localhost:4321/perry
  client->Write(a, strlen(a));
  memset( a, 0 , MAXBUF );

  client->Read(a, MAXBUF);
  std::cout << a << std::endl;
}



// void NachOS_Socket() {			// System call 30
//   printf("Socket inicia\n");
//   int isIpPv6 = machine->ReadRegister(4);
//   int socketType = machine->ReadRegister(5);


//   isIpPv6 = (isIpPv6 == AF_INET_NachOS) ? AF_INET : AF_INET6;
//   socketType = (socketType == SOCK_STREAM_NachOS) ? SOCK_DGRAM : SOCK_STREAM;  

//   int idSocket = socket(isIpPv6, socketType, 0);
//   int fd = currentThread->openFilesTable->Open(idSocket);
//   machine->WriteRegister(2, fd);

//   if (isIpPv6 == AF_INET6) {
//     socketMap.insert({fd, true});
//   }
//   printf("Socket inicia\n");
//   returnFromSystemCall();
// }





// void NachOS_Connect() {		// System call 31
//   int idSocket = machine->ReadRegister(4);
//   int ipAddress = machine->ReadRegister(5);
//   int port = machine->ReadRegister(6);

//   char* hostip = new char[40];
//   for (int i = 0; i < 40; ++i) {
//     machine->ReadMem(ipAddress+i, 1, (int*)&hostip[i]);
//     if (hostip[i] == '\0') {
//       break;
//     }
//   }

//   int fd = currentThread->openFilesTable->getUnixHandle(idSocket);
//   auto it = socketMap.find(fd);

//   int st = 0;

//   struct sockaddr_in  host4;
//   struct sockaddr_in6  host6;
//   struct sockaddr * ha;

//   if (it == socketMap.end()) {
//     memset( (char *) &host4, 0, sizeof( host4 ) );
//     host4.sin_family = AF_INET;
//     st = inet_pton( AF_INET, hostip, &host4.sin_addr );
//     if ( -1 == st ) {
//       throw( std::runtime_error( "Nachos_Connect(): ipv4 inet" ));
//     }

//     host4.sin_port = htons( port );
//     st = connect( idSocket, (sockaddr *) &host4, sizeof( host4 ) );
//     if ( -1 == st ) {
//       throw( std::runtime_error(  "Nachos_Connect(): ipv4 connect" ));
//     }
//   } else {
//     memset( &host6, 0, sizeof( host6 ) );
//     host6.sin6_family = AF_INET6;
//     st = inet_pton( AF_INET6, hostip, &host6.sin6_addr );
//     if ( -1 == st ) {	// 0 means invalid address, -1 means address error
//       throw std::runtime_error( "Nachos_Connect(): ipv6 inet" );
//     }

//     host6.sin6_port = htons( port );
//     ha = (struct sockaddr *) &host6;
//     st = connect( idSocket, ha, sizeof( host6 ) );
//     if ( -1 == st ) {	// 0 means invalid address, -1 means address error
//       throw std::runtime_error( "Nachos_Connect(): ipv6 connect" );
//     }
//   }
// }
