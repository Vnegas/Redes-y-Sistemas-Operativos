/**
 *   UCR-ECCI
 *   CI-0123 Proyecto integrador de redes y sistemas operativos
 *
 *   Socket client/server example with threads
 *
 **/

#include <thread>
#include <cstdlib>	// atoi
#include <cstdio>	// printf
#include <string>
#include <cstring>	// strlen, strcmp
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <filesystem>
#include "json.hpp"
#include <vector>
#include <dirent.h>
#include <regex>
#include <sys/socket.h>
#include "Socket.h"

#define PORT 4321



void sendResponse(Socket* socket, const char* response) {
  // Create the HTTP response headers
  std::string http_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  // Concatenate the headers and response into a single string
  std::string combined_response = http_header + response;
  // Send the headers and response
  socket->Write(combined_response.c_str(), combined_response.size());
}

void get_Regex(std::string code_html, std::string exp_regular, std::vector<std::string>& code_html_vector){
  std::regex pattern(exp_regular);
  std::smatch matches;
  while (std::regex_search(code_html, matches, pattern)) {
    code_html_vector.push_back(matches[1]);
    code_html = matches.suffix().str();
  }
}

void browser(char* buf, std::vector<std::string>& html_vector) {
  get_Regex(buf, "GET /([A-Za-z]+) HTTP/1.1", std::ref(html_vector));
}

void Service( Socket * client ) {
  char buf[ 1024 ] = { 0 };
  int sd, bytes;

  std::string directorio = "./jsons/";
  std::vector<std::string> archivos;

  // Abre el directorio
  DIR* dir = opendir(directorio.c_str());
  if (!dir) {
    std::cerr << "No se pudo abrir el directorio." << std::endl;
    return;
  }
  int total = 0;
  // Lee los nombres de los archivos .txt en el directorio
  struct dirent* entry;
  while ((entry = readdir(dir))) {
    std::string nombreArchivo = entry->d_name;
    if (nombreArchivo.size() > 5 && nombreArchivo.substr(nombreArchivo.size() - 5) == ".json") {
      nombreArchivo = nombreArchivo.substr(0, nombreArchivo.size() - 5);
      archivos.push_back(nombreArchivo);
    }
  }
  closedir(dir);
  std::string options = "";
  for(int i = 0 ; i < archivos.size(); i++){
    options += ( archivos[i] + ",").c_str();
  }
  
  client->Read( buf, sizeof( buf ) );
  std::string request(buf);

  if (request.find("Intermediary") != std::string::npos) {
    client->Write(options.c_str(), options.size());
    client->Close();
    return;
  }
  // std::cout << request << std::endl;
  if (request.find("User-Agent: ") != std::string::npos) {
    std::cout << "Desde navegador" << std::endl;
    bool lego = false;
    for (int i = 0; i < archivos.size(); ++i) {
      if (request.find("GET /" + archivos[i]) != std::string::npos) {
        // printf("Lego found\n");
        lego = true;
        break;
      }
    }

    // std::cout << std::endl << std::endl << request << std::endl;

    if (lego) {
      printf("Lego page\n");

      std::vector<std::string> html_vector;
      browser(buf, html_vector);
      memset( buf, 0, 1024 );
      strcpy(buf, html_vector[0].c_str());

      //! Para abrir el json a partir de buf que contiene el nombre del animal
      std::string fileName = buf;
      std::ifstream inputFile("./jsons/" + fileName + ".json");
      printf("Filename: %s\n", fileName.c_str());
      if (!inputFile.is_open()) {
        std::cerr << "Failed to open the JSON file." << std::endl;
      }

      nlohmann::json json_data;
      inputFile >> json_data;
      
      inputFile.close();
      std::string json_string = json_data.dump();
      nlohmann::json jfigure = json_data[buf];
      std::string hmtl = "<!DOCTYPE html> \
      <html> \
        <head> \
          <title>Lego</title>\
          <style>\
            h1 {\
              font-size: 34px;\
              margin-left: 8px;\
            }\
            p {\
              font-size: 24px;\
              margin-left: 5px;\
            }\
          </style>\
        </head>\
        <body>\
        ";
        std::string nameFigure = buf;
        hmtl+= "<h1>" + nameFigure + "</h1>\
        <ul> ";
        std::cout << jfigure.size() << "\n";
        for (auto i = jfigure.begin(); i != jfigure.end(); ++i) {
          hmtl += "<li style=\"font-size: 26px; margin-left: 35;\">" + i.key()  +"</li>";
        }
        hmtl += "<hr><p><a href=\"/ \">Back</a></p>\n \
      </html>";
      lego = false;
      sendResponse(client, hmtl.c_str());
    } else if (request.find("GET / ") != std::string::npos) {
      printf("Home page\n");
      std::string hmtl;
      hmtl += "<!DOCTYPE html> \
      <html> \
        <head> \
          <title>HOME PAGE</title>\
          <style>\
            h1 {\
              font-size: 46px;\
            }\
            .custom-text {\
              font-size: 28px;\
              margin-left: 30px;\
            }\
            .custom-opt {\
              font-size: 20px;\
            }\
            img {\
              width: 300px;\
              height: auto;\
              display: block;\
              margin: 75px auto 0;\
              position: center;\
            }\
          </style>\
          <script>\
          function changeURL() { \
            var selection = document.getElementById(\"options\"); \
            var opSelected = selection.options[selection.selectedIndex].value; \
            window.location.href = \"/\" + opSelected; \
          } \
          </script>\
        </head>\
        <body>\
            <h1> <center> Malvados&&Asociados .Inc </center> </h1>\
      </html>";

      hmtl += "<label for=\"options\" class=\"custom-text\">Lista de Figuras&nbsp;&nbsp;<label>\
                <select id=\"options\" name=\"options\" onchange=\"changeURL()\">";
                  hmtl += "<option value=Figura class=\"custom-opt\">Seleccionar Figura</option>";
                  for (int i = 0; i < archivos.size(); ++i) {
                    hmtl += "<option value=\"" + archivos[i] + "\" class=\"custom-opt\">" + archivos[i] + "</option>";
                  }
      hmtl += "</select> \
          <img src=\"https://wallpaperset.com/w/full/7/d/f/422504.jpg\" alt=\"Perry\">\
        </body> \
      </html>";

      sendResponse(client, hmtl.c_str());
    } else {
      // Error page
      std::cout << "Error page" << std::endl;

      std::string hmtl = "<!DOCTYPE html>\n \
        <html lang=\"en\">\n \
          <meta charset=\"ascii\"/>\n \
          <title> Page not Found </title>\n \
          <style>body {font-size: 34px;} .err {color: red}</style>\n \
            <h1 class=\"err\"> Page not Found </h1>\n \
            <p style=\"font-size: 24px;margin-left: 15px;\">Invalid request</p>\n \
            <hr><p style=\"font-size: 24px;margin-left: 15px;\"><a href=\"/ \">Back</a></p>\n \
        </html>";

      const char* http_header = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n";

      sendResponse(client,hmtl.c_str());
      // client->Write(http_header, strlen(http_header));
      // client->Write(hmtl.c_str(), strlen(hmtl.c_str()));
    }

    client->Close();
    
  } else if (request.find("Accept: ") != std::string::npos) {
    std::string json_string;
    std::cout << "Desde terminal" << std::endl;
    std::vector<std::string> html_vector;
    browser(buf, html_vector);
    memset( buf, 0, 1024 );
    strcpy(buf, html_vector[0].c_str());

    //! Para abrir el json a partir de buf que contiene el nombre del animal
    std::string fileName = buf;
    std::ifstream input_file("./jsons/" + fileName + ".json");
    printf("Filename: %s\n", fileName.c_str());

    if (!input_file.is_open()) {
      json_string = "NOT FOUND";
      client->Write(json_string.c_str(), strlen(json_string.c_str()));    
    } else {
      nlohmann::json json_data;
      input_file >> json_data;
      input_file.close();
      json_string = json_data.dump();
      std::regex regex("[{}\"]");
      std::string output = std::regex_replace(json_string, regex, "\0");
      std::regex regex2("[,]");
      std::string output2 = std::regex_replace(output, regex2, "\n");
      client->Write(output2.c_str(), strlen(output2.c_str()));
    }
    client->Close();
  }
}

int main( int cuantos, char ** argumentos ) {
  Socket * server, * client;
  std::thread * worker;
  int port = PORT;
  
  if ( cuantos > 1 ) {
    port = atoi( argumentos[ 1 ] );
  }

  server = new Socket( 's' );

  server->Bind( port );
  server->Listen( 10 );

  while(1) {
    client = server->Accept();
    std::cout << "Client" << client << std::endl;
    worker = new std::thread( Service, client );	// service connection
  }
}
