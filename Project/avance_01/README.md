# Client-Server

### Table of Contents
1. [Description](#description)
2. [Programas](#programs)
    - [Client](#client)
    - [Simulation](#simulation)
3. [User manual](#user-manual)
    - [Client manual](#client-manual)
    - [Simulation manual](#simulation-manual)
4. [Figure](#figure)
    - [Pieces](#pieces)
5. [Credits](#credits)

### Description

for this program we create a client that is able to connect to a server with a variety of lego sets, the program reads the available sets and lists them for the user to select one of these sets, then the program returns a list of the pieces needed to build the lego figure. 

In this repository there is also a program that simulates with threads an intermediary and a server that will be in charge of providing the figures and pieces to the client, which will be later implemented.

### Programs

#### Client

<!-- TODO: Explain Client -->

#### Simulation


In the simulation program two threads are created, the first represents an intermediary between the user and the server and the second represents the server, if when running the program some parameter is added to it, the server will be raised first, if it is not entered no parameters so the broker is raised first, once both threads are up the server sends the data of the lego figure and its parts to the broker which displays it to the server

### User manual

#### Client manual

First you have to run the makefile on the terminal:

```
../client/src$ make
```

Once the program is compiled, you can run it on the terminal
```
../client/src$ ./client.out
```

#### Simulation manual

First you have to run the makefile on the terminal:

```
../simulation$ make
```

Once the program is compiled, you can run it on the terminal
```
../simulation$ bin/simulation
```

### Figure

It's a Platypus!

![figure](/avance_01/lego_figure/platypus.png)

#### Pieces:

[Pieces ](/avance_01/lego_figure/pieces.md)

### Credits

Marlon Murillo 
Daniel Van Der Laat
Lisandro Sandi
Sebastian Venegas