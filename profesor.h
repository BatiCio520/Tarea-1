#ifndef PROFESOR_H
#define PROFESOR_H

#include <string>
#include <iostream>

using namespace std;

// --- Constantes ---
const int MAX_PREGUNTAS = 100;
const string NOMBRE_ARCHIVO_PREGUNTAS = "bancoDePreguntas.txt"; 
const int NIVELES_TAXONOMIA = 6;
const string NIVELES[NIVELES_TAXONOMIA] = {
    "Recordar", "Entender", "Aplicar", "Analizar", "Evaluar", "Crear"
};
const int NUM_TIPOS_PREGUNTA = 5;
const string TIPOS_PREGUNTA[NUM_TIPOS_PREGUNTA] = {
    "Opción Múltiple", "Verdadero/Falso", "Respuesta Corta",
    "Composición/Ensayo", "Emparejamiento"
};


// --- Clase Pregunta  ---
class Pregunta {
private:
    int id;
    string enunciado;
    string tipo;
    string nivelTaxonomico;
    int tiempoEstimado;
    string solucion;
public:
    Pregunta();
    Pregunta(int id, string enunciado, string tipo, string nivel, int tiempo, string solucion);
    // Getters (Ahora const)
    int getId() const;
    string getEnunciado() const;
    string getTipo() const;
    string getNivelTaxonomico() const;
    int getTiempoEstimado() const; 
    string getSolucion() const; 
    void setId(int id);
    void setEnunciado(const string& enunciado);
    void setTipo(const string& tipo);
    void setNivelTaxonomico(const string& nivel);
    void setTiempoEstimado(int tiempo);
    void setSolucion(const string& solucion);
    // Mostrar (También debe ser const)
    void mostrar() const;
};

// --- Clase Profesor ---
class Profesor {
private:
    Pregunta bancoDePreguntas[MAX_PREGUNTAS];
    int numeroDePreguntasActual;
    int proximoId;

    // Funciones auxiliares
    int buscarIndicePreguntaPorId(int id) const;
    bool validarNivelTaxonomico(const string& nivel) const;
    bool validarTipoPregunta(const string& tipo) const;
    void limpiarBufferEntrada() const;
    
    void cargarPreguntas();
    void guardarPreguntas();
    

public:
    Profesor(); 
    ~Profesor(); 

    
    void mostrarMenu();
    void ejecutarOpcion(int opcion);
    void agregarPregunta();
    void consultarPreguntas();
    void actualizarPregunta();
    void borrarPregunta();
    void buscarPreguntasPorTaxonomia();
    void generarEvaluacion();
};

#endif