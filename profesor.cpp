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
int Pregunta::getId() { return id; }
string Pregunta::getEnunciado() { return enunciado; }
string Pregunta::getTipo() { return tipo; }
string Pregunta::getNivelTaxonomico() { return nivelTaxonomico; }
int Pregunta::getTiempoEstimado() { return tiempoEstimado; }
string Pregunta::getSolucion() { return solucion; }

//setters
void Pregunta::setId(int id) { this->id = id; }
void Pregunta::setEnunciado(const string& enunciado) { this->enunciado = enunciado; }  
void Pregunta::setTipo(const string& tipo) { this->tipo = tipo; }
void Pregunta::setNivelTaxonomico(const string& nivel) { this->nivelTaxonomico = nivel; }
void Pregunta::setTiempoEstimado(int tiempo) { this->tiempoEstimado = tiempo; }
void Pregunta::setSolucion(const string& solucion) { this->solucion = solucion; }
void Pregunta::mostrar() {
    cout << "----------------------------------------\n";
    cout << "ID: " << getId() << "\n";
    cout << "Enunciado: " << getEnunciado() << "\n";
    cout << "Tipo: " << getTipo() << "\n";
    cout << "Nivel Taxonómico: " << getNivelTaxonomico() << "\n";
    cout << "Tiempo Estimado: " << getTiempoEstimado() << " minutos\n";
    cout << "Solución: " << getSolucion() << "\n";
    cout << "----------------------------------------\n";
}

// --- Clase profesor
Profesor::Profesor() : numeroDePreguntasActual(0), proximoId(1) {
    cargarPreguntas();
}
Profesor::~Profesor() {
    guardarPreguntas();
}
void Profesor::limpiarBufferEntrada() const{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

}
bool Profesor::validarNivelTaxonomico(const string& nivel) const{
    for(int i=0; i<NIVELES_TAXONOMIA; i++) if(NIVELES[i]== nivel) return true;
    return false;
}
bool Profesor::validarTipoPregunta(const string& tipo) const{
    for(int i=0; i<NUM_TIPOS_PREGUNTA; i++) if(TIPOS_PREGUNTA[i] == tipo) return true;
    return false;
    
}
int Profesor::buscarIndicePreguntaPorId(int id) const {
    for (int i = 0; i < numeroDePreguntasActual; ++i) if (bancoDePreguntas[i].getId() == id) return i;
    return -1;
}

// Función para cargar preguntas desde un archivo
void Profesor::cargarPreguntas() {
    ifstream archivoEntrada(bancoDePreguntas.txt);
    if (!archivoEntrada.is_open()) {
        cout << "Advertencia: No se encontró '" << bancoDePreguntas.txt << "'. Iniciando vacío." << endl;
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
    cout << "Info: Se cargaron " << numeroDePreguntasActual << " preguntas desde '" << NOMBRE_ARCHIVO_PREGUNTAS << "'." << endl;
}

