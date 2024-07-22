/**
 *
 *   UCR-ECCI
 *
 *   IPv4 TCP client normal or SSL according to parameters
 *
 **/

#include "syscall.h"

/**
 * @brief Create sockets to get info from server's lego and parse that
 * info using regular expressions.
 * #define ConsoleInput	0  
    #define ConsoleOutput	1  
    #define ConsoleError	2  
 */

int main( int argc, char * argv[] ) {
    int id;
    char a[ 128 ];
    Write( a, 128, 1 );
    id = Socket( AF_INET_NachOS, SOCK_STREAM_NachOS );
    Connect( id, "10.1.137.5", 4321 );
    char* terminal = "terminal";
    Write(terminal, 9, id );
    a[0] = '\0';
    Read( a, 128, id );
    Write( a, 128, 1 );

    //! Enviar la figura elegida
    char* figura;
    Read(figura, 25, 0);
    Write(figura, 25, id );

    char piezas[256];
    Read( piezas, 256, id );
    Write( piezas, 256, 1 );
    Write( "\n", 2, 1 );

}
