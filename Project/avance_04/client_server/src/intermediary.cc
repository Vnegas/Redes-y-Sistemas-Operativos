/**
 *   UCR-ECCI
 *   CI-0123 Proyecto integrador de redes y sistemas operativos
 *
 *   Socket client/server example with threads
 *
 **/

#include <thread>
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <sys/socket.h>
#include <map>
#include <regex>
#include <vector>

#include <arpa/inet.h> 
#include <netinet/in.h>
#include "Socket.h"

#define PORT 2023

struct protocol_uwu {
  int action;
  char figures[1024];
  char ip[256];
};

Socket * server, * intermediary, * client, * interRcv, * interSnd, * inter_inter;
std::map<std::string, std::string> ourLegoTable;
std::map<std::string, std::string> otherLegoTable;
std::vector<char*> broadcastAddrs;
struct protocol_uwu uwu;

void parseFiguresOther(std::string a, std::string host) {
  std::string piece;
  int pos = 0; std::string delimiter = ",";
  while ((pos = a.find(delimiter)) != std::string::npos) {
    piece = a.substr(0, pos);
    otherLegoTable.insert(std::pair<std::string,std::string>(piece, host));
    a.erase(0, pos + delimiter.length());
  }
}

void parseFigures(std::string a, std::string host) {
  std::string piece;
  int pos = 0; std::string delimiter = ",";
  while ((pos = a.find(delimiter)) != std::string::npos) {
    piece = a.substr(0, pos);
    ourLegoTable.insert(std::pair<std::string,std::string>(piece, host));
    a.erase(0, pos + delimiter.length());
  }
}

void delFigures(std::string a, std::string host) {
  std::string piece;
  int pos = 0; std::string delimiter = ",";
  while ((pos = a.find(delimiter)) != std::string::npos) {
    piece = a.substr(0, pos);
    otherLegoTable.erase(piece);
    a.erase(0, pos + delimiter.length());
  }
}

void Broadcast() {
  // Create sockets to send and receive
  interSnd = new Socket( 'd' );
  interRcv = new Socket( 'd' );
  // Save specific IP for broadcast send
  broadcastAddrs.push_back("172.16.123.31");
  broadcastAddrs.push_back("172.16.123.63");
  broadcastAddrs.push_back("172.16.123.79");

  BroadcastSend();
  while (1) {
    // Listen for new income
    BroadcastRecv();
    // If received something, send. It means that a new inter has arrived
    BroadcastSend();
  }
}

void BroadcastSend() {
  // Set broadcast
  struct sockaddr_in other;
  memset( &other, 0, sizeof( other ) );
  other.sin_family = AF_INET;
  other.sin_port = htons(PORT);
  // Send broadcast to specific IPs
  for (int i = 0; i < broadcastAddrs.size(); ++i) {
    other.sin_addr.s_addr = broadcastAddrs[i];
    interSnd->sendTo(static_cast<void*>(&uwu), sizeof(uwu), (void *) & other);
  }
}

void BroadcastRecv() {
  // Set broadcast
  struct sockaddr_in other;
  memset( &other, 0, sizeof( other ) );
  other.sin_family = AF_INET;
  other.sin_port = htons(PORT);
  other.sin_addr.s_addr = INADDR_ANY;
  // Buffer to receive data
  char buffer[sizeof(uwu) + 1];

  interRcv->recvFrom( (void *) buffer, sizeof(uwu), (void *) & other );
  int action = buffer[1];
  std::string uwu_struct(buffer); uwu_struct.erase(0, 1);
  size_t found = uwu_struct.find_first_of("0123456789");
  std::string IPstring = uwu_struct.substr(found, strlen(buffer)-1);
  // Extracts figures from buffer (before number)
  std::string figures = uwu_struct.substr(0, found);
  // What action to take
  switch (action) {
    case 1: // Add
      parseFiguresOther(figures, IPstring);
      break;
    case 2: // Del
      delFigures(figures, IPstring);
      break;
    case 3: // Del
      delFigures(figures, IPstring);
      break;
    
    default:
      break;
  }
}

void get_Regex(std::string code_html, std::string exp_regular, std::vector<std::string>& code_html_vector){
  std::regex pattern(exp_regular);
  std::smatch matches;
  while (std::regex_search(code_html, matches, pattern)) {
    code_html_vector.push_back(matches[1]);
    code_html = matches.suffix().str();
  }
}

void getLego(char* buf, std::vector<std::string>& html_vector) {
  get_Regex(buf, "GET /([A-Za-z]+) HTTP/1.1", std::ref(html_vector));
}

void Service (Socket * client) {
  char pieces[1024] = { 0 };
  char choice[1024] = { 0 };
  char buf[ 1024 ] = { 0 };
  char piece_chosen[1024] = { 0 };
  char response[2048] = { 0 };
  
  client->Read(buf,sizeof(buf));
  std::string check(buf);
  
  // Check if requested figure is on our server

  if (check.find("GET /favicon.ico") == std::string::npos) {
    // Check if request is homePage
    if (check.find("GET / ") != std::string::npos) {
      server->Write(buf,sizeof(buf));
      server->Read(response, sizeof(response));
      client->Write(response, sizeof(response));
      server->Close();
      client->Close();
    } else {
      // Find lego name in request
      std::vector<std::string> vector_lego;
      char legoName[ 512 ] = { 0 };
      getLego(buf, vector_lego);
      memset( legoName, 0, 1024 );
      strcpy(legoName, vector_lego[0].c_str());
      std::string legoPiece = legoName;
      // Check if lego is in our table or other table. If not send to our server (error page)
      std::map<std::string,std::string>::iterator it = otherLegoTable.find(legoPiece);
      if (it != otherLegoTable.end()) {
        int port = 2023;
        char * hostname = (char *) it->second.c_str();
        inter_inter = new Socket('s', false);
        inter_inter->Connect( hostname, port );
        inter_inter->Write(buf,strlen(buf));
        inter_inter->Read(response,sizeof(response));
        
        client->Write(response, sizeof(response));
        inter_inter->Close();
        server->Close();
        client->Close();
      } else {
        server->Write(buf,sizeof(buf));
        server->Read(response, sizeof(response));
        client->Write(response, sizeof(response));
        server->Close();
        client->Close();
      }
    }
  }
}

void Connect_Server(int x) {
  int port = 4321;
  char * hostname = (char *) "192.168.84.90"; std::string host(hostname);
  server = new Socket( 's' );
  server->Connect( hostname, port );
  // Ask for pieces
  if (x == 0) {
    char* key = (char *) "Intermediary";
    server->Write(key, strlen(key));
    char vectorPieces[2048] = { 0 };
    server->Read(vectorPieces, sizeof(vectorPieces));
    // Set Struct for UDP
    strcpy(uwu.figures, vectorPieces);
    strcpy(uwu.ip, hostname);
    uwu.action = 1;
    // Divide string by delimiter and save on table
    std::string a(vectorPieces); parseFigures(a, host);
    // Print table
    std::map<std::string,std::string>::iterator it = ourLegoTable.begin();
    for (it = ourLegoTable.begin(); it != ourLegoTable.end(); ++it) {
      std::cout << it->first << " => " << it->second << '\n';
    }
  }
}

void Connect_Client (int cuantos, char** argumentos) {
  std::thread * worker;
  int port = PORT;

  if ( cuantos > 1 ) {
    port = atoi( argumentos[ 1 ] );
  }

  intermediary = new Socket( 's' );

  intermediary->Bind( port );
  intermediary->Listen( 10 );

  int x = 2;

  while(1) {
    Connect_Server(1);
    client = intermediary->Accept();
    std::cout << "Client" << client << std::endl;
    worker = new std::thread( Service, client );	// service connection
  }
}

int main( int cuantos, char ** argumentos ) {
  Connect_Server(0);
  // Broadcast
  std::thread * workerSnd = new std::thread( Broadcast );	// broadcast connection
  Connect_Client(cuantos, argumentos);
}
