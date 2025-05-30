#include "profesor.h"
#include <limits>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
// --- Clase pregunta --- //
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
    
    cout << "Nivel Taxonomico: " << getNivelTaxonomico() << "\n";
    cout << "Tiempo Estimado: " << getTiempoEstimado() << " minutos\n";
    
    cout << "Solucion: " << getSolucion() << "\n";
    cout << "----------------------------------------\n";
}

// --- Implementacion Clase Profesor --- 
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
        
        cerr << "Error: No se pudo abrir '" << NOMBRE_ARCHIVO_PREGUNTAS << "' para guardar." << endl;
        return;
    }
    // Itera usando numeroDePreguntasActual y usa los getters de Pregunta
    for (int i = 0; i < numeroDePreguntasActual; ++i) {
        const Pregunta& p = bancoDePreguntas[i]; 
        archivoSalida << p.getId() << "|" << p.getTiempoEstimado() << "|"
                      << p.getTipo() << "|" << p.getNivelTaxonomico() << "|"
                      << p.getEnunciado() << "|" << p.getSolucion() << "\n";
    }
    archivoSalida.close();
    
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

// Funcion para cargar preguntas desde un archivo 
void Profesor::cargarPreguntas() {
    ifstream archivoEntrada(NOMBRE_ARCHIVO_PREGUNTAS);
    if (!archivoEntrada.is_open()) {
        
        cout << "Advertencia: No se encontro '" << NOMBRE_ARCHIVO_PREGUNTAS << "'. Iniciando vacio." << endl;
        return;
    }
    string linea;
    int maxId = 0;
    while (getline(archivoEntrada, linea)) {
        if (numeroDePreguntasActual >= MAX_PREGUNTAS) {
            
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
            
            cerr << "Advertencia: Error de conversion numerica en linea, saltando: " << linea << " (" << e.what() << ")" << endl;
        } catch (const std::out_of_range& e) {
             
             cerr << "Advertencia: Numero fuera de rango en linea, saltando: " << linea << " (" << e.what() << ")" << endl;
        } catch (...) {
             
             cerr << "Advertencia: Error desconocido al procesar linea, saltando: " << linea << endl;
        }
    }
    archivoEntrada.close();
    proximoId = maxId + 1;
    
    cout << "Info: Se cargaron " << numeroDePreguntasActual << " preguntas desde '" << NOMBRE_ARCHIVO_PREGUNTAS << "'." << endl;
}

void Profesor::mostrarMenu() {
    
    cout << "\n--- Gestion de Preguntas para Evaluaciones ---\n";
    cout << "1. Agregar Nueva Pregunta\n";
    cout << "2. Consultar Todas las Preguntas\n";
    cout << "3. Actualizar Pregunta por ID\n";
    cout << "4. Borrar Pregunta por ID\n";
    
    cout << "5. Buscar Preguntas por Nivel Taxonomico\n";
    
    cout << "6. Generar Evaluacion\n";
    cout << "0. Salir\n";
    cout << "---------------------------------------------\n";
    
    cout << "Ingrese su opcion: ";
}
void Profesor::ejecutarOpcion(int opcion) {
    switch (opcion) {
        case 1: agregarPregunta(); break;
        case 2: consultarPreguntas(); break; 
        case 3: actualizarPregunta(); break;
        case 4: borrarPregunta(); break;
        case 5: buscarPreguntasPorTaxonomia(); break;
        case 6: generarEvaluacion(); break; 
        case 0: cout << "Saliendo del programa...\n"; break;
        
        default: cout << "Opcion no valida. Intente de nuevo.\n"; break;
    }
}
void Profesor::agregarPregunta() {
    if (numeroDePreguntasActual >= MAX_PREGUNTAS) {
        
        cout << "Error: Limite maximo de preguntas alcanzado.\n"; return;
    }
    string n_enunciado, n_tipo, n_nivel, n_solucion; int n_tiempo;
    
    cout << "\n--- Agregar Nueva Pregunta ---\n";
    limpiarBufferEntrada();
    cout << "Ingrese el enunciado: "; getline(cin, n_enunciado);
    // Usa validarTipoPregunta y constantes 
    do {
        cout << "Ingrese el tipo (";
        for(int i = 0; i < NUM_TIPOS_PREGUNTA; ++i) cout << TIPOS_PREGUNTA[i] << (i == NUM_TIPOS_PREGUNTA - 1 ? "" : " / ");
        cout << "): "; getline(cin, n_tipo);
        
        if (!validarTipoPregunta(n_tipo)) cout << "Tipo invalido.\n";
    } while (!validarTipoPregunta(n_tipo));
    // Usa validarNivelTaxonomico y constantes 
     do {
        
        cout << "Ingrese el nivel taxonomico (";
        for(int i=0; i < NIVELES_TAXONOMIA; ++i) cout << NIVELES[i] << (i == NIVELES_TAXONOMIA - 1 ? "" : " / ");
        cout << "): "; getline(cin, n_nivel);
         
         if (!validarNivelTaxonomico(n_nivel)) cout << "Nivel invalido.\n";
    } while (!validarNivelTaxonomico(n_nivel));
    // Valida tiempo y usa limpiarBufferEntrada
    cout << "Ingrese el tiempo estimado (minutos): ";
    while (!(cin >> n_tiempo) || n_tiempo <= 0) {
        
        cout << "Entrada invalida. Numero positivo: "; cin.clear(); limpiarBufferEntrada();
    } limpiarBufferEntrada();
    
    cout << "Ingrese la solucion esperada: "; getline(cin, n_solucion);
    // Usa los setters de Pregunta
    Pregunta& nuevaPregunta = bancoDePreguntas[numeroDePreguntasActual];
    nuevaPregunta.setId(proximoId); nuevaPregunta.setEnunciado(n_enunciado); nuevaPregunta.setTipo(n_tipo);
    nuevaPregunta.setNivelTaxonomico(n_nivel); nuevaPregunta.setTiempoEstimado(n_tiempo); nuevaPregunta.setSolucion(n_solucion);
    numeroDePreguntasActual++; proximoId++; // Actualiza contadores
    
    cout << "Pregunta agregada con exito (ID: " << proximoId - 1 << ").\n";
}

void Profesor::consultarPreguntas() {
    
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
    
    while (!(cin >> id) || id <= 0) { cout << "ID invalido. Positivo: "; cin.clear(); limpiarBufferEntrada(); }
    limpiarBufferEntrada();
    int indice = buscarIndicePreguntaPorId(id);
    
    if (indice == -1) { cout << "Error: Pregunta con ID " << id << " no encontrada.\n"; return; }
    
    cout << "Pregunta encontrada. Nuevos datos (deje en blanco para no cambiar):\n";
    string entrada; Pregunta& pregunta = bancoDePreguntas[indice];
    // Usa getters para mostrar valor actual y setters para actualizar
    cout << "Nuevo enunciado [" << pregunta.getEnunciado() << "]: "; getline(cin, entrada);
    if (!entrada.empty()) pregunta.setEnunciado(entrada);
    cout << "Nuevo tipo [" << pregunta.getTipo() << "] (...): "; getline(cin, entrada);
    
    if (!entrada.empty()) { if (validarTipoPregunta(entrada)) pregunta.setTipo(entrada); else cout << "Tipo invalido, no se actualizo.\n"; }
    
    cout << "Nuevo nivel taxonomico [" << pregunta.getNivelTaxonomico() << "] (...): "; getline(cin, entrada);
    
    if (!entrada.empty()) { if (validarNivelTaxonomico(entrada)) pregunta.setNivelTaxonomico(entrada); else cout << "Nivel invalido, no se actualizo.\n"; }
    cout << "Nuevo tiempo estimado (minutos) [" << pregunta.getTiempoEstimado() << "]: "; getline(cin, entrada);
    if (!entrada.empty()) {
        try { int nuevoTiempo = std::stoi(entrada); if (nuevoTiempo > 0) pregunta.setTiempoEstimado(nuevoTiempo);
        
        else cout << "Tiempo invalido, no se actualizo.\n"; }
        
        catch (...) { cout << "Entrada no numerica para tiempo, no se actualizo.\n"; }
    }
    
    cout << "Nueva solucion [" << pregunta.getSolucion() << "]: "; getline(cin, entrada);
    if (!entrada.empty()) pregunta.setSolucion(entrada);
    
    cout << "Pregunta actualizada con exito.\n";
}
void Profesor::borrarPregunta() {
    int id; cout << "\n--- Borrar Pregunta por ID ---\n";
    cout << "Ingrese el ID de la pregunta a borrar: ";
    
    while (!(cin >> id) || id <= 0) { cout << "ID invalido. Positivo: "; cin.clear(); limpiarBufferEntrada(); }
    limpiarBufferEntrada();
    int indice = buscarIndicePreguntaPorId(id);
    
    if (indice == -1) { cout << "Error: Pregunta con ID " << id << " no encontrada.\n"; return; }
    for (int i = indice; i < numeroDePreguntasActual - 1; ++i) bancoDePreguntas[i] = bancoDePreguntas[i + 1];
    numeroDePreguntasActual--;
    
    cout << "Pregunta con ID " << id << " borrada con exito.\n";
}

void Profesor::buscarPreguntasPorTaxonomia() const {
    string nivelBuscado;
    
    cout << "\n--- Buscar Preguntas por Nivel Taxonomico ---\n";
    limpiarBufferEntrada();
     do {
        
        cout << "Ingrese el nivel taxonomico a buscar (";
        for(int i=0; i < NIVELES_TAXONOMIA; ++i) cout << NIVELES[i] << (i == NIVELES_TAXONOMIA - 1 ? "" : " / ");
        cout << "): ";
        getline(cin, nivelBuscado);
         
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

void Profesor::generarEvaluacion() {
    int cantidadPreguntas; string nivelDeseado;
    
    cout << "\n--- Generar Evaluacion ---\n";
    if (numeroDePreguntasActual == 0) { cout << "No hay preguntas en el banco.\n"; return; }
    cout << "Ingrese la cantidad de preguntas deseadas: ";
    
    while (!(cin >> cantidadPreguntas) || cantidadPreguntas <= 0) { cout << "Numero invalido. Positivo: "; cin.clear(); limpiarBufferEntrada(); }
    limpiarBufferEntrada();
     do {
        
        cout << "Ingrese el nivel taxonomico deseado (...): "; getline(cin, nivelDeseado);
         
         if (!validarNivelTaxonomico(nivelDeseado)) cout << "Nivel invalido.\n";
    } while (!validarNivelTaxonomico(nivelDeseado));
    
    cout << "\n--- Evaluacion Generada (Nivel: " << nivelDeseado << ") ---\n";
    int preguntasIncluidas = 0; int tiempoTotalEstimado = 0;
    // Usa getter para comparar y getter para sumar tiempo
    for (int i = 0; i < numeroDePreguntasActual && preguntasIncluidas < cantidadPreguntas; ++i) {
        if (bancoDePreguntas[i].getNivelTaxonomico() == nivelDeseado) {
            bancoDePreguntas[i].mostrar(); tiempoTotalEstimado += bancoDePreguntas[i].getTiempoEstimado(); preguntasIncluidas++;
        }
    }
    // Muestra resultados 
    if (preguntasIncluidas == 0) cout << "No se encontraron preguntas del nivel '" << nivelDeseado << "'.\n";
    else if (preguntasIncluidas < cantidadPreguntas) {
        
        cout << "Advertencia: Solo se encontraron " << preguntasIncluidas << " preguntas.\n";
         cout << "Tiempo total estimado: " << tiempoTotalEstimado << " minutos.\n";
    } else {
        cout << "Se incluyeron " << preguntasIncluidas << " preguntas.\n";
        cout << "Tiempo total estimado: " << tiempoTotalEstimado << " minutos.\n";
    }
     cout << "-----------------------------------------------------\n";
}