#include "profesor.h"
#include <limits>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
// --- Clase pregunta --- // Comentario sin acentos
Pregunta::Pregunta() : id(0), tiempoEstimado(0) {}
Pregunta::Pregunta(int id, string enunciado, string tipo, string nivel, int tiempo, string sol)
    : id(id), enunciado(enunciado), tipo(tipo), nivelTaxonomico(nivel), tiempoEstimado(tiempo), solucion(sol) {}


//getters
int Pregunta::getId() const { return id; }
string Pregunta::getEnunciado() const { return enunciado; }
string Pregunta::getTipo() const { return tipo; }
string Pregunta::getNivelTaxonomico() const { return nivelTaxonomico; }
int Pregunta::getTiempoEstimado() const { return tiempoEstimado; }
string Pregunta::getSolucion() const { return solucion; }


//setters
void Pregunta::setId(int id) { this->id = id; }
void Pregunta::setEnunciado(const string& enunciado) { this->enunciado = enunciado; }
void Pregunta::setTipo(const string& tipo) { this->tipo = tipo; }
void Pregunta::setNivelTaxonomico(const string& nivel) { this->nivelTaxonomico = nivel; }
void Pregunta::setTiempoEstimado(int tiempo) { this->tiempoEstimado = tiempo; }
void Pregunta::setSolucion(const string& solucion) { this->solucion = solucion; }
void Pregunta::mostrar() const {
    cout << "----------------------------------------\n";
    cout << "ID: " << getId() << "\n";
    cout << "Enunciado: " << getEnunciado() << "\n";
    cout << "Tipo: " << getTipo() << "\n";
    // Sin acento
    cout << "Nivel Taxonomico: " << getNivelTaxonomico() << "\n";
    cout << "Tiempo Estimado: " << getTiempoEstimado() << " minutos\n";
    // Sin acento
    cout << "Solucion: " << getSolucion() << "\n";
    cout << "----------------------------------------\n";
}

// --- Implementacion Clase Profesor --- // Sin acento
Profesor::Profesor() : numeroDePreguntasActual(0), proximoId(1) {
    // Llama a cargarPreguntas usando la constante definida en profesor.h
    cargarPreguntas();
}
Profesor::~Profesor() {
    // Llama a guardarPreguntas usando la constante definida en profesor.h
    guardarPreguntas();
}
void Profesor::guardarPreguntas() {
    ofstream archivoSalida(NOMBRE_ARCHIVO_PREGUNTAS);
    if (!archivoSalida.is_open()) {
        // Sin acento
        cerr << "Error: No se pudo abrir '" << NOMBRE_ARCHIVO_PREGUNTAS << "' para guardar." << endl;
        return;
    }
    // Itera usando numeroDePreguntasActual y usa los getters de Pregunta
    for (int i = 0; i < numeroDePreguntasActual; ++i) {
        const Pregunta& p = bancoDePreguntas[i]; // Usar const Pregunta& es mas correcto aqui
        archivoSalida << p.getId() << "|" << p.getTiempoEstimado() << "|"
                      << p.getTipo() << "|" << p.getNivelTaxonomico() << "|"
                      << p.getEnunciado() << "|" << p.getSolucion() << "\n";
    }
    archivoSalida.close();
    // Sin acentos
     cout << "Info: Banco de preguntas guardado en '" << NOMBRE_ARCHIVO_PREGUNTAS << "'." << endl;
}

void Profesor::limpiarBufferEntrada() const {
     // Usa std::numeric_limits (requiere <limits>)
     cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
}
bool Profesor::validarNivelTaxonomico(const string& nivel) const {

    for(int i = 0; i < NIVELES_TAXONOMIA; ++i) if (NIVELES[i] == nivel) return true;
    return false;
}
bool Profesor::validarTipoPregunta(const string& tipo) const {

     for(int i = 0; i < NUM_TIPOS_PREGUNTA; ++i) if (TIPOS_PREGUNTA[i] == tipo) return true;
    return false;
}
int Profesor::buscarIndicePreguntaPorId(int id) const {
    // Usa el getter de Pregunta
    for (int i = 0; i < numeroDePreguntasActual; ++i) if (bancoDePreguntas[i].getId() == id) return i;
    return -1;
}

// Funcion para cargar preguntas desde un archivo // Sin acento
void Profesor::cargarPreguntas() {
    ifstream archivoEntrada(NOMBRE_ARCHIVO_PREGUNTAS);
    if (!archivoEntrada.is_open()) {
        // Sin acento
        cout << "Advertencia: No se encontro '" << NOMBRE_ARCHIVO_PREGUNTAS << "'. Iniciando vacio." << endl;
        return;
    }
    string linea;
    int maxId = 0;
    while (getline(archivoEntrada, linea)) {
        if (numeroDePreguntasActual >= MAX_PREGUNTAS) {
            // Sin acento
            cerr << "Advertencia: Limite de preguntas (" << MAX_PREGUNTAS << ") alcanzado al cargar." << endl;
            break;
        }
        stringstream ss(linea);
        string campo;
        int id = 0, tiempo = 0;
        string tipo, nivel, enunciado, solucion;

        try {
            if (!getline(ss, campo, '|') || campo.empty()) continue;

            id = std::stoi(campo);

            if (!getline(ss, campo, '|') || campo.empty()) continue;
            tiempo = std::stoi(campo);

            if (!getline(ss, tipo, '|')) continue;
            if (!getline(ss, nivel, '|')) continue;
            if (!getline(ss, enunciado, '|')) continue;
            if (!getline(ss, solucion)) continue;

            if (id <= 0 || tiempo <= 0 || !validarTipoPregunta(tipo) || !validarNivelTaxonomico(nivel)) {
                 // Sin acentos
                 cerr << "Advertencia: Datos invalidos en linea, saltando: " << linea << endl;
                 continue;
            }

            Pregunta& p = bancoDePreguntas[numeroDePreguntasActual];
            p.setId(id); p.setTiempoEstimado(tiempo); p.setTipo(tipo);
            p.setNivelTaxonomico(nivel); p.setEnunciado(enunciado); p.setSolucion(solucion);
            numeroDePreguntasActual++;
            if (id > maxId) maxId = id;

        // Capturar excepciones especificas
        } catch (const std::invalid_argument& e) {
            // Sin acentos
            cerr << "Advertencia: Error de conversion numerica en linea, saltando: " << linea << " (" << e.what() << ")" << endl;
        } catch (const std::out_of_range& e) {
             // Sin acentos
             cerr << "Advertencia: Numero fuera de rango en linea, saltando: " << linea << " (" << e.what() << ")" << endl;
        } catch (...) {
             // Sin acentos
             cerr << "Advertencia: Error desconocido al procesar linea, saltando: " << linea << endl;
        }
    }
    archivoEntrada.close();
    proximoId = maxId + 1;
    // Sin acentos
    cout << "Info: Se cargaron " << numeroDePreguntasActual << " preguntas desde '" << NOMBRE_ARCHIVO_PREGUNTAS << "'." << endl;
}

void Profesor::mostrarMenu() {
    // Sin acentos
    cout << "\n--- Gestion de Preguntas para Evaluaciones ---\n";
    cout << "1. Agregar Nueva Pregunta\n";
    cout << "2. Consultar Todas las Preguntas\n";
    cout << "3. Actualizar Pregunta por ID\n";
    cout << "4. Borrar Pregunta por ID\n";
    // Sin acento
    cout << "5. Buscar Preguntas por Nivel Taxonomico\n";
    // Sin acento
    cout << "6. Generar Evaluacion\n";
    cout << "0. Salir\n";
    cout << "---------------------------------------------\n";
    // Sin acento
    cout << "Ingrese su opcion: ";
}
void Profesor::ejecutarOpcion(int opcion) {
    switch (opcion) {
        case 1: agregarPregunta(); break;
        case 2: consultarPreguntas(); break; // Nota: Esta funcion no era const en el codigo provisto por el usuario
        case 3: actualizarPregunta(); break;
        case 4: borrarPregunta(); break;
        case 5: buscarPreguntasPorTaxonomia(); break;
        case 6: generarEvaluacion(); break; // Nota: Esta funcion no era const en el codigo provisto por el usuario
        case 0: cout << "Saliendo del programa...\n"; break;
        // Sin acentos
        default: cout << "Opcion no valida. Intente de nuevo.\n"; break;
    }
}
void Profesor::agregarPregunta() {
    if (numeroDePreguntasActual >= MAX_PREGUNTAS) {
        // Sin acentos
        cout << "Error: Limite maximo de preguntas alcanzado.\n"; return;
    }
    string n_enunciado, n_tipo, n_nivel, n_solucion; int n_tiempo;
    // Sin acentos
    cout << "\n--- Agregar Nueva Pregunta ---\n";
    limpiarBufferEntrada();
    cout << "Ingrese el enunciado: "; getline(cin, n_enunciado);
    // Usa validarTipoPregunta y constantes (ahora sin acentos)
    do {
        cout << "Ingrese el tipo (";
        for(int i = 0; i < NUM_TIPOS_PREGUNTA; ++i) cout << TIPOS_PREGUNTA[i] << (i == NUM_TIPOS_PREGUNTA - 1 ? "" : " / ");
        cout << "): "; getline(cin, n_tipo);
        // Sin acento
        if (!validarTipoPregunta(n_tipo)) cout << "Tipo invalido.\n";
    } while (!validarTipoPregunta(n_tipo));
    // Usa validarNivelTaxonomico y constantes (ahora sin acentos)
     do {
        // Sin acento
        cout << "Ingrese el nivel taxonomico (";
        for(int i=0; i < NIVELES_TAXONOMIA; ++i) cout << NIVELES[i] << (i == NIVELES_TAXONOMIA - 1 ? "" : " / ");
        cout << "): "; getline(cin, n_nivel);
         // Sin acento
         if (!validarNivelTaxonomico(n_nivel)) cout << "Nivel invalido.\n";
    } while (!validarNivelTaxonomico(n_nivel));
    // Valida tiempo y usa limpiarBufferEntrada
    cout << "Ingrese el tiempo estimado (minutos): ";
    while (!(cin >> n_tiempo) || n_tiempo <= 0) {
        // Sin acentos
        cout << "Entrada invalida. Numero positivo: "; cin.clear(); limpiarBufferEntrada();
    } limpiarBufferEntrada();
    // Sin acento
    cout << "Ingrese la solucion esperada: "; getline(cin, n_solucion);
    // Usa los setters de Pregunta
    Pregunta& nuevaPregunta = bancoDePreguntas[numeroDePreguntasActual];
    nuevaPregunta.setId(proximoId); nuevaPregunta.setEnunciado(n_enunciado); nuevaPregunta.setTipo(n_tipo);
    nuevaPregunta.setNivelTaxonomico(n_nivel); nuevaPregunta.setTiempoEstimado(n_tiempo); nuevaPregunta.setSolucion(n_solucion);
    numeroDePreguntasActual++; proximoId++; // Actualiza contadores
    // Sin acento
    cout << "Pregunta agregada con exito (ID: " << proximoId - 1 << ").\n";
}
// Nota: Esta funcion no era const en el codigo provisto por el usuario
void Profesor::consultarPreguntas() {
    // Sin acento ("banco" en minuscula como en el codigo provisto)
    cout << "\n--- banco de Preguntas Actual ("<<numeroDePreguntasActual<<" preguntas) ---\n";
    if (numeroDePreguntasActual == 0) {
        cout << "No hay preguntas registradas.\n"; return;
    }
    for (int i = 0; i < numeroDePreguntasActual; ++i) {
        bancoDePreguntas[i].mostrar(); // Usa el metodo mostrar de Pregunta
    }
}
void Profesor::actualizarPregunta(){
    int id; cout <<"\n--- Actualizar Pregunta por ID ---\n";
    cout << "Ingrese el ID de la pregunta a actualizar: ";
    // Sin acento
    while (!(cin >> id) || id <= 0) { cout << "ID invalido. Positivo: "; cin.clear(); limpiarBufferEntrada(); }
    limpiarBufferEntrada();
    int indice = buscarIndicePreguntaPorId(id);
    // Sin acento
    if (indice == -1) { cout << "Error: Pregunta con ID " << id << " no encontrada.\n"; return; }
    // Sin acento
    cout << "Pregunta encontrada. Nuevos datos (deje en blanco para no cambiar):\n";
    string entrada; Pregunta& pregunta = bancoDePreguntas[indice];
    // Usa getters para mostrar valor actual y setters para actualizar
    cout << "Nuevo enunciado [" << pregunta.getEnunciado() << "]: "; getline(cin, entrada);
    if (!entrada.empty()) pregunta.setEnunciado(entrada);
    cout << "Nuevo tipo [" << pregunta.getTipo() << "] (...): "; getline(cin, entrada);
    // Sin acentos
    if (!entrada.empty()) { if (validarTipoPregunta(entrada)) pregunta.setTipo(entrada); else cout << "Tipo invalido, no se actualizo.\n"; }
    // Sin acento
    cout << "Nuevo nivel taxonomico [" << pregunta.getNivelTaxonomico() << "] (...): "; getline(cin, entrada);
    // Sin acentos
    if (!entrada.empty()) { if (validarNivelTaxonomico(entrada)) pregunta.setNivelTaxonomico(entrada); else cout << "Nivel invalido, no se actualizo.\n"; }
    cout << "Nuevo tiempo estimado (minutos) [" << pregunta.getTiempoEstimado() << "]: "; getline(cin, entrada);
    if (!entrada.empty()) {
        try { int nuevoTiempo = std::stoi(entrada); if (nuevoTiempo > 0) pregunta.setTiempoEstimado(nuevoTiempo);
        // Sin acentos
        else cout << "Tiempo invalido, no se actualizo.\n"; }
        // Sin acentos
        catch (...) { cout << "Entrada no numerica para tiempo, no se actualizo.\n"; }
    }
    // Sin acento
    cout << "Nueva solucion [" << pregunta.getSolucion() << "]: "; getline(cin, entrada);
    if (!entrada.empty()) pregunta.setSolucion(entrada);
    // Sin acento
    cout << "Pregunta actualizada con exito.\n";
}
void Profesor::borrarPregunta() {
    int id; cout << "\n--- Borrar Pregunta por ID ---\n";
    cout << "Ingrese el ID de la pregunta a borrar: ";
    // Sin acento
    while (!(cin >> id) || id <= 0) { cout << "ID invalido. Positivo: "; cin.clear(); limpiarBufferEntrada(); }
    limpiarBufferEntrada();
    int indice = buscarIndicePreguntaPorId(id);
    // Sin acento
    if (indice == -1) { cout << "Error: Pregunta con ID " << id << " no encontrada.\n"; return; }
    for (int i = indice; i < numeroDePreguntasActual - 1; ++i) bancoDePreguntas[i] = bancoDePreguntas[i + 1];
    numeroDePreguntasActual--;
    // Sin acento
    cout << "Pregunta con ID " << id << " borrada con exito.\n";
}
// Nota: Esta funcion SI era const en el codigo provisto por el usuario
void Profesor::buscarPreguntasPorTaxonomia() const {
    string nivelBuscado;
    // Sin acento
    cout << "\n--- Buscar Preguntas por Nivel Taxonomico ---\n";
    limpiarBufferEntrada();
     do {
        // Sin acento
        cout << "Ingrese el nivel taxonomico a buscar (";
        for(int i=0; i < NIVELES_TAXONOMIA; ++i) cout << NIVELES[i] << (i == NIVELES_TAXONOMIA - 1 ? "" : " / ");
        cout << "): ";
        getline(cin, nivelBuscado);
         // Sin acento
         if (!validarNivelTaxonomico(nivelBuscado)) cout << "Nivel invalido. Elija uno de la lista.\n";
    } while (!validarNivelTaxonomico(nivelBuscado));

    cout << "\n--- Preguntas encontradas para el nivel '" << nivelBuscado << "' ---\n";
    int encontradas = 0;
    // Usa getter de Pregunta para comparar
    for (int i = 0; i < numeroDePreguntasActual; ++i) {
        if (bancoDePreguntas[i].getNivelTaxonomico() == nivelBuscado) {
            bancoDePreguntas[i].mostrar(); // Usa mostrar() const
            encontradas++;
        }
    }
    if (encontradas == 0) {
        cout << "No se encontraron preguntas para este nivel.\n";
    } else {
        cout << "Total encontradas: " << encontradas << "\n";
    }
}
// Nota: Esta funcion NO era const en el codigo provisto por el usuario
void Profesor::generarEvaluacion() {
    int cantidadPreguntas; string nivelDeseado;
    // Sin acento
    cout << "\n--- Generar Evaluacion ---\n";
    if (numeroDePreguntasActual == 0) { cout << "No hay preguntas en el banco.\n"; return; }
    cout << "Ingrese la cantidad de preguntas deseadas: ";
    // Sin acentos
    while (!(cin >> cantidadPreguntas) || cantidadPreguntas <= 0) { cout << "Numero invalido. Positivo: "; cin.clear(); limpiarBufferEntrada(); }
    limpiarBufferEntrada();
     do {
        // Sin acento
        cout << "Ingrese el nivel taxonomico deseado (...): "; getline(cin, nivelDeseado);
         // Sin acento
         if (!validarNivelTaxonomico(nivelDeseado)) cout << "Nivel invalido.\n";
    } while (!validarNivelTaxonomico(nivelDeseado));
    // Sin acento
    cout << "\n--- Evaluacion Generada (Nivel: " << nivelDeseado << ") ---\n";
    int preguntasIncluidas = 0; int tiempoTotalEstimado = 0;
    // Usa getter para comparar y getter para sumar tiempo
    for (int i = 0; i < numeroDePreguntasActual && preguntasIncluidas < cantidadPreguntas; ++i) {
        if (bancoDePreguntas[i].getNivelTaxonomico() == nivelDeseado) {
            bancoDePreguntas[i].mostrar(); tiempoTotalEstimado += bancoDePreguntas[i].getTiempoEstimado(); preguntasIncluidas++;
        }
    }
    // Muestra resultados (sin acentos)
    if (preguntasIncluidas == 0) cout << "No se encontraron preguntas del nivel '" << nivelDeseado << "'.\n";
    else if (preguntasIncluidas < cantidadPreguntas) {
        // Sin acento
        cout << "Advertencia: Solo se encontraron " << preguntasIncluidas << " preguntas.\n";
         cout << "Tiempo total estimado: " << tiempoTotalEstimado << " minutos.\n";
    } else {
        cout << "Se incluyeron " << preguntasIncluidas << " preguntas.\n";
        cout << "Tiempo total estimado: " << tiempoTotalEstimado << " minutos.\n";
    }
     cout << "-----------------------------------------------------\n";
}