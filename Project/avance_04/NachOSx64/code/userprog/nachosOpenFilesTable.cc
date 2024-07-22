#include "nachosOpenFilesTable.h"
#define MAX_OPEN_FILES 5


NachosOpenFilesTable::NachosOpenFilesTable() {
    // MAX_OPEN_FILES
    openFilesMap = new BitMap(MAX_OPEN_FILES);
    openFilesMap->Mark(0); //Set nth bit in a bitmap. Reserved
	openFilesMap->Mark(1); // Reserved
	openFilesMap->Mark(2); // Reserved
    this->usage = 1; // Cuantos hilos usan la tabla
    openFiles = new int[MAX_OPEN_FILES]; // Creando el arreglo de fd's
    // Inicializar todos los elementos del arreglo a cero
    // for (int i = 0; i < MAX_OPEN_FILES; i++) {
    //     openFiles[i] = -1;
    // }
    // Reservados para input, output y stderr
    openFiles[0] = 0;
	openFiles[1] = 1;
	openFiles[2] = 2;

} // Initialize
NachosOpenFilesTable::~NachosOpenFilesTable() {
    delete openFiles;
    delete openFilesMap;
} // De-allocate
int NachosOpenFilesTable::getUsage() {
    return this->usage;
}
int NachosOpenFilesTable::Open( int UnixHandle ) {

    int handle = this->openFilesMap->Find(); // Retorna el numero del primer bit despejado y lo establece en uno

    if (handle == -1) {
        return -1;
    }
    this->openFiles[handle] = UnixHandle;
    return handle;


    // int handle = 0;

    // for (int openFilesIndex = 0;
    //     openFilesIndex < MAX_OPEN_FILES;
    //     openFilesIndex++) {
    //         printf("Llamando al open de la tabla f \n");

    //     if (!this->openFilesMap->Test(openFilesIndex)) {
    //         printf("Llamando al open de la tabla if1\n");

    //         continue;
    //     }

    //     if (this->openFiles[openFilesIndex] == UnixHandle) {
    //         printf("Llamando al open de la tabla if2\n");
    //         handle = openFilesIndex;
    //         break;
    //     }
    // }
    // printf("Llamando al open de la tabla\n");

    // if (handle != 0) {
    //     return handle;
    // }

    // handle = this->openFilesMap->Find();

    // if (handle == -1) {
    //     return -1;
    // }

    // this->openFiles[handle] = UnixHandle;

    // return handle;

} // Register the file handle

int NachosOpenFilesTable::Close( int NachosHandle ) {

    if (isOpened(NachosHandle)) {
        int UnixHandle = this->openFiles[NachosHandle];
        openFilesMap->Clear(NachosHandle);
        this->openFiles[NachosHandle] = -1;
        return UnixHandle; // Retorna el fd de unix del archivo que se cerro
	} else {
		return -1; // de lo contrario retorna -1
	}    

} // Unregister the file handle
bool NachosOpenFilesTable::isOpened( int NachosHandle ) {
    if( this->openFiles[NachosHandle] != -1 && this->openFilesMap->Test(NachosHandle)) {
        return true;
    }
    return false;
    
}
int NachosOpenFilesTable::getUnixHandle( int NachosHandle ) {
    return this->openFiles[NachosHandle];
}
void NachosOpenFilesTable::addThread() {
    this->usage++;
}							// If a user thread is using this table, add it
void NachosOpenFilesTable::delThread() {
    this->usage--;
}							// If a user thread is using this table, delete it

void NachosOpenFilesTable::Print() {
    for(int i=0; i< MAX_OPEN_FILES; i++) {
        if (this->isOpened(i)) {
            printf("%d\n", openFiles[i]);
        }
    }
}	