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

void sendResponse(int fd, const char* response) {
  // Create the HTTP response headers
  const char* http_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";

  // Send the headers
  send(fd, http_header, strlen(http_header), 0);

  // Send the HTML content
  send(fd, response, strlen(response), 0);
}

void sendResponse(Socket* socket, const char* response) {
  // Create the HTTP response headers
  const char* http_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";

  // Send the headers
  socket->Write(http_header, strlen(http_header));

  socket->Write(response, strlen(response));
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
      options += ( archivos[i] + "\n").c_str();
  }
  
  client->Read( buf, sizeof( buf ) );
  std::string buffer = buf;
  size_t found = buffer.find("perry");

  if (found!=std::string::npos) {
    std::cout << "Buff desde figura: " << buf << std::endl;
    std::cout << "Desde navegador figura" << std::endl;
    std::vector<std::string> html_vector;
    browser(buf, html_vector);
    memset( buf, 0, 1024 );
    strcpy(buf, html_vector[0].c_str());
    std::cout << "figura elegida: " << buf << std::endl;

    //! Para abrir el json a partir de buf que contiene el nombre del animal
    std::string fileName = buf;
    if(fileName == "perry") {
      std::string fileTest = "./jsons/" + fileName + ".json";
      std::ifstream input_file("./jsons/" + fileName + ".json");

      if (!input_file.is_open()) {
        std::cerr << "Failed to open the JSON file." << std::endl;
      }

      nlohmann::json json_data;
      input_file >> json_data;
      input_file.close();
      std::string json_string = json_data.dump();
      nlohmann::json jfigure = json_data[buf];
      std::string hmtl;
      std::string http_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
      hmtl += http_header;
      hmtl += "<!DOCTYPE html> \
      <html> \
        <head> \
          <title>Lista de Palabras</title>\
        </head>\
        <body>\
        ";
        std::string nameFigure = buf;
        hmtl+= "<h1>" + nameFigure + "</h1>\
        <ul> ";
        for (auto i = jfigure.begin(); i != jfigure.end(); ++i) {
          hmtl += "<li>" + i.key() +"</li>";
        }
        hmtl += "\
        </html>";

      //sendResponse(client, hmtl.c_str());
      client->Write(hmtl.c_str(), strlen(hmtl.c_str()));
    }


    
  } else if(found==std::string::npos) {
    std::cout << "Buff desde home: " << buf << std::endl;
    std::cout << "Desde navegador home" << std::endl;
    std::string hmtl;
    std::string http_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    hmtl += http_header;
    hmtl += "<!DOCTYPE html> \
    <html> \
      <head> \
        <title>HOME PAGE</title>\
      </head>\
      <body>\
      <h1> HOME </h1>";
      hmtl += "\
      </html>";

    // sendResponse(client, hmtl.c_str());
 
    client->Write(hmtl.c_str(), strlen(hmtl.c_str()));

  } else {
    std::cout << "Desde terminal" << std::endl;
    client->Write( options.c_str(), strlen(options.c_str()) );
    memset( buf, 0, 1024 );
    client->Read( buf, sizeof( buf ) );
    
    std::string fileName = buf;
    std::string fileTest = "./jsons/" + fileName + ".json";
    std::ifstream input_file("./jsons/" + fileName + ".json");

    if (!input_file.is_open()) {
      std::cerr << "Failed to open the JSON file." << std::endl;
    }

    nlohmann::json json_data;
    input_file >> json_data;
    input_file.close();
    std::string json_string = json_data.dump();
    client->Write(json_string.c_str(), strlen(json_string.c_str()));
  }

  client->Close();
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
