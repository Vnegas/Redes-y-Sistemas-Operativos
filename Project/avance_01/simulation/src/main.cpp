// Copyright 2023 <Marvin Lisandro, Sebastian Venegas,
// Daniel Van Der Laat, Marlon Murillo>

#include <unistd.h>
#include <semaphore.h>

#include <thread>
#include <iostream>
#include <string>




typedef struct shared_data {
  sem_t sem;          /**< Semaphore for synchronization. */
  std::string figure; /**< Name of the figure. */
  std::string pieces; /**< Description of the figure's pieces. */
  int start;          /**< if 1, starts intermediary, else starts server. */
  std::string ip;     /**< IP address for communication. */
  std::string port;   /**< Port number for communication. */
} shared_data_t;

/**
 * @brief This function acts as an intermediary in the simulation.
 *
 * If the value of `start` in `shared_data` is equal to 0, it waits for the signal and starts
 * as an intermediary. Otherwise, it waits for a short period, then starts as an intermediary,
 * and finally waits for the signal.
 *
 * @param shared_data A pointer to the shared data structure `shared_data_t`.
 */
void inter(shared_data_t* shared_data);

/**
 * @brief This function acts as a server in the simulation.
 *
 * If the value of `start` in `shared_data` is equal to 0, it waits for a short period,
 * then starts as a server. Otherwise, it waits for the signal, and then sets up the
 * IP address, port, and other data in `shared_data` before releasing the signal.
 *
 * @param shared_data A pointer to the shared data structure `shared_data_t`.
 */
void server(shared_data_t* shared_data);

int main(int argc, char *argv[]) {
  (void) argv;
  shared_data_t *shared_data = new shared_data_t;
  sem_init(&shared_data->sem, 0, 0);
  shared_data->figure = "";
  shared_data->pieces = "";
  shared_data->start = 1;
  shared_data->ip = "";
  shared_data->port = "";

  if (argc < 2) {
    shared_data->start = 0;
  }

  std::thread inter_thread(inter, shared_data);
  std::thread server_thread(server, shared_data);

  inter_thread.join();
  server_thread.join();

  sem_destroy(&shared_data->sem);
  delete shared_data;    
  return 0;
}

void inter(shared_data_t* shared_data) {
  if (shared_data->start == 0) {
    std::cout << "Starting intermediary" << std::endl;
    sem_wait(&shared_data->sem);
  } else {
    sleep(1.5);
    std::cout << "Starting intermediary in second place" << std::endl;
    sem_post(&shared_data->sem);
    sleep(1.5);
  }
  sem_wait(&shared_data->sem);
  std::cout << "\nFigure: " << shared_data->figure << std::endl;
  std::cout << "Pieces: " << shared_data->pieces << std::endl;
}

void server(shared_data_t* shared_data) {
  if (shared_data->start == 0) {
    sleep(1.5);
    std::cout << "Starting server in second place" << std::endl;
    sem_post(&shared_data->sem);
  } else {
    std::cout << "Starting server" << std::endl;
    sem_wait(&shared_data->sem);
  }
  shared_data->ip = "123.12.07.12";
  shared_data->port = "8080";
  shared_data->figure = "Platypus\n";
  shared_data->pieces = "1 brick 2x1 blue "
   "\n        2 plate 2x1 orange \n        2 plate 2x1 blue "
  "\n        2 plate 4x2 blue  \n        2 plate 3x2 orange"
   "\n        1 brick 3x2 blue \n        2 brick 1x1 blue eye";
  sem_post(&shared_data->sem);
}
