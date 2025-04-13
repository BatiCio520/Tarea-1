#include "profesor.h" 
#include <limits>    

int main() {
    Profesor miGestor;
    int opcion = -1; 

    // Bucle principal del menú
    do {
        miGestor.mostrarMenu(); // Muestra las opciones al usuario

        // Leer y validar la opción ingresada por el usuario
        while (!(cin >> opcion)) { // Mientras la lectura falle (no sea un número)
            cout << "Entrada inválida. Por favor ingrese un número: ";
            cin.clear(); // Limpiar el estado de error de cin
            // Descartar la entrada incorrecta del buffer hasta el salto de línea
            cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
        }
        if (opcion != 0) {
            miGestor.ejecutarOpcion(opcion);
        }

    } while (opcion != 0); // Repetir mientras no se elija la opción 0 (Salir)


    return 0; // Fin del programa
}
