#include "profesor.h"
#include <limits>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "bancoDePreguntas.txt"
// --- Clase pregunta
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
    cout << "Nivel Taxonómico: " << getNivelTaxonomico() << "\n";
    cout << "Tiempo Estimado: " << getTiempoEstimado() << " minutos\n";
    cout << "Solución: " << getSolucion() << "\n";
    cout << "----------------------------------------\n";
}

// --- Implementación Clase Profesor ---
Profesor::Profesor() : numeroDePreguntasActual(0), proximoId(1) {
    // Llama a cargarPreguntas usando la constante definida en profesor.h
    cargarPreguntas();
}
Profesor::~Profesor() {
    // Llama a guardarPreguntas usando la constante definida en profesor.h
    guardarPreguntas();
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

// Función para cargar preguntas desde un archivo
void Profesor::cargarPreguntas() {
    ifstream archivoEntrada(NOMBRE_ARCHIVO_PREGUNTAS);
    if (!archivoEntrada.is_open()) {
        cout << "Advertencia: No se encontró '" << NOMBRE_ARCHIVO_PREGUNTAS << "'. Iniciando vacío." << endl;
        return;
    }
    string linea;
    int maxId = 0;
    while (getline(archivoEntrada, linea)) {
        if (numeroDePreguntasActual >= MAX_PREGUNTAS) {
            cerr << "Advertencia: Límite de preguntas (" << MAX_PREGUNTAS << ") alcanzado al cargar." << endl;
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
                 cerr << "Advertencia: Datos inválidos en línea, saltando: " << linea << endl;
                 continue;
            }

            Pregunta& p = bancoDePreguntas[numeroDePreguntasActual];
            p.setId(id); p.setTiempoEstimado(tiempo); p.setTipo(tipo);
            p.setNivelTaxonomico(nivel); p.setEnunciado(enunciado); p.setSolucion(solucion);
            numeroDePreguntasActual++;
            if (id > maxId) maxId = id;

        // Capturar excepciones específicas
        } catch (const std::invalid_argument& e) {
            cerr << "Advertencia: Error de conversión numérica en línea, saltando: " << linea << " (" << e.what() << ")" << endl;
        } catch (const std::out_of_range& e) {
             cerr << "Advertencia: Número fuera de rango en línea, saltando: " << linea << " (" << e.what() << ")" << endl;
        } catch (...) {
             cerr << "Advertencia: Error desconocido al procesar línea, saltando: " << linea << endl;
        }
    }
    archivoEntrada.close();
    proximoId = maxId + 1;
    cout << "Info: Se cargaron " << numeroDePreguntasActual << " preguntas desde '" << bancoDePreguntas << "'." << endl;
}

void Profesor::mostrarMenu() { 
    cout << "\n--- Gestión de Preguntas para Evaluaciones ---\n";
    cout << "1. Agregar Nueva Pregunta\n";
    cout << "2. Consultar Todas las Preguntas\n";
    cout << "3. Actualizar Pregunta por ID\n";
    cout << "4. Borrar Pregunta por ID\n";
    cout << "5. Buscar Preguntas por Nivel Taxonómico\n";
    cout << "6. Generar Evaluación\n";
    cout << "0. Salir\n";
    cout << "---------------------------------------------\n";
    cout << "Ingrese su opción: ";
}