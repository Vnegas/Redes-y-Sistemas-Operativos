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
