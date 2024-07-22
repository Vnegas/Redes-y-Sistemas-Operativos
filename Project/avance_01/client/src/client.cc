/**
 *
 *   UCR-ECCI
 *
 *   IPv4 TCP client normal or SSL according to parameters
 *
 **/

#include <stdio.h>
#include <string.h>
#include <cstring>
#include "VSocket.h"
#include "Socket.h"
#include "SSLSocket.h"
#include <iostream>
#include <string>
#include <regex>
#include <cctype> 

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
/**
 * @brief Create sockets to get info from server's lego and parse that
 * info using regular expressions.
 */
int main( int argc, char * argv[] ) {
   VSocket * client;
   int st, port = 443;
   char a[ MAXBUF ];
   char * os = (char *) "163.178.104.187";
   std::string lego = (char *) "GET /lego/index.php HTTP/1.1\r\nhost: redes.ecci\r\n\r\n"; 
//   char * request = (char *) "GET /ci0123 HTTP/1.1\r\nhost:redes.ecci\r\n\r\n";


   client = new SSLSocket(false);	// Create a new stream socket for IPv4

   memset( a, 0 , MAXBUF );
   client->Connect( os, port );
   client->Write(  lego.c_str(), lego.length() );
   int count = 0;
   std::string html_text;
   while ((count = client->Read( a, MAXBUF )) > 0 ) {
        html_text += a; // fills string with the htmls
        memset( a, 0, MAXBUF );
   }

   // std::cout << html_text << std::endl;


   std::vector<std::string> code_html;
   getRegex(html_text, "<OPTION value=\"([A-Za-z]+)\">", code_html);

   for (size_t i = 1; i < code_html.size(); i++)
   {
      std::cout << i << ". " << code_html[i] << std::endl;
   }
   client->Close();

   std::string numberChoosed;
   int number = 0;
   int numberCorrect = 0;
   

   while (numberCorrect == 0)
   {
      std::cout << "Elige un numero para seleccionar la figura: " << std::endl;
      std::cin >> numberChoosed;
      if (isNumber(numberChoosed))
      {
         number = std::stoi(numberChoosed);
         if(number > code_html.size() || number < 1 ){
            std::cout << "Debe seleccionar un numero de las figuras disponibles" << std::endl;
         } else {
            std::cout << "Por favor, espere..." << std::endl;
            numberCorrect = 1;
         }
      } else {
         std::cout << "No se aceptan caracteres" << std::endl;
      }
      
      
   }
   
   
   std::string figure = "/lego/list.php?figure=" + code_html[number];

   lego = "GET " + figure + " HTTP/1.1\r\nhost: redes.ecci\r\n\r\n";

   // std::cout << lego << std::endl;
   VSocket * client2;
   client2 = new SSLSocket();

   memset(a, 0, MAXBUF);
   client2->Connect(os, port);
   client2->Write(lego.c_str(), lego.length());
   count = 0;
   std::string html_figure;
   while ((count = client2->Read(a, MAXBUF)) > 0) {
       html_figure += a;
       memset(a, 0, MAXBUF);
   }
   // std::cout << html_figure << std::endl;
   std::vector<std::string> vector_html_figures;
   getRegex(html_figure, "<TD ALIGN=center> ([A-Za-z0-9 ]+)</TD>", vector_html_figures);

   std::cout << "--------------------------" << std::endl;
   for (size_t i = 0; i < vector_html_figures.size(); i++)
   {
      if(i%2 == 0) {
         std::cout << vector_html_figures[i] << " ";

      } else {
         std::cout << vector_html_figures[i] << std::endl;
         std::cout << "--------------------------" << std::endl;
      }
   }
}