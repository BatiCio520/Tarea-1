#include "profesor.h"
#include <limits>
#include <iostream> 

int main() {
    Profesor miGestor;
    int opcion = -1;

    do {
        miGestor.mostrarMenu();

        
        while (!(cin >> opcion)) {
            cout << "Entrada invalida. Por favor ingrese un numero: "; 
            cin.clear();
            cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
        }

        if (opcion != 0) {
            miGestor.ejecutarOpcion(opcion);
        }

    } while (opcion != 0);

    return 0;
}