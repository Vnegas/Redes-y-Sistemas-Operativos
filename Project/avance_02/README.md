# Client-Server

### Table of Contents
1. [Description](#description)
3. [User manual](#user-manual)
    - [Client manual](#client-manual)
    - [server manual](#server-manual)
4. [Figure](#figure)
    - [Pieces](#pieces)
5. [Credits](#credits)

### Description

For this program, we created a server and a client. The server is capable of receiving requests from both a terminal client and a web browser client. The client can connect to the server with a variety of Lego games. The program reads the available games from .json files and lists them for the user to select one of these games. Then, the server returns a list of the pieces necessary to build the Lego figure.

### User manual

#### Client manual

First you have to run the makefile on the terminal:

```
../client_server/src$ make
```

Once the program is compiled, you can run it on the terminal
```
../client_server/src$ ./client.out
```

#### Server manual

First you have to run the makefile on the terminal:

```
../client_server$ make
```

Once the program is compiled, you can run it on the terminal
```
../client_server$ sudo ./server
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