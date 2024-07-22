#include "syscall.h"

int
main()
{
    // OpenFileId input;
    // OpenFileId output;
    // char buffer[1024];
    // int n = 0;
    
    Create( "nachos.file40" );
    int nachosFd = Open("nachos.file40");
    char* textToWrite = "Hello World!";
    Write(textToWrite, 5, nachosFd); // void Write(char *buffer, int size, OpenFileId id);
    // printf("%d bytes escritos\n", bytesWritten);
    // Halt();
    //Exit( 0 );
}
