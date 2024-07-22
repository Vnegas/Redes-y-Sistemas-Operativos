// #include <iostream>
// #include <string>
// #include <fstream>
// #include <filesystem>
// #include "json.hpp"
// #include <vector>
// #include <dirent.h>

// int main() {
//     std::string directorio = "./jsons/";
//     std::vector<std::string> archivos;

//     // Abre el directorio
//     DIR* dir = opendir(directorio.c_str());
//     if (!dir) {
//         std::cerr << "No se pudo abrir el directorio." << std::endl;
//         return 1;
//     }
//     int total = 0;
//     // Lee los nombres de los archivos .txt en el directorio
//     struct dirent* entry;
//     while ((entry = readdir(dir))) {
//         std::string nombreArchivo = entry->d_name;
//         if (nombreArchivo.size() > 5 && nombreArchivo.substr(nombreArchivo.size() - 5) == ".json") {
//             nombreArchivo = nombreArchivo.substr(0, nombreArchivo.size() - 5);
//             archivos.push_back(nombreArchivo);
//         }
//     }
//     closedir(dir);
//     for(int i = 0 ; i < archivos.size(); i++){
//         std::cout << archivos[i] << std::endl;
//     }

//     int figura = 0;
//     std::cout << "elija un numero: ";
//     std::cin >> figura ;
//     std::string contenido;
//     std::ifstream input_file(directorio + archivos[figura-1] + ".json");
    
//     if (input_file) {

//         if (!input_file.is_open()) {
//             std::cerr << "Failed to open the JSON file." << std::endl;
//             return 1; // Return an error code
//         }

//         nlohmann::json json_data;
//         input_file >> json_data;
//         input_file.close();
//         contenido = json_data.dump();
//         input_file.close();
        
//     } else {
//         std::cerr << "No se pudo abrir el archivo: " << archivos[figura-1] << std::endl << std::endl;
//     }

//     std::cout << archivos[figura-1] << " = " << contenido << std::endl;

// }
