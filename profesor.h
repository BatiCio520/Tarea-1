#ifndef PROFESOR_H
#define PROFESOR_H

#include <string>
#include <iostream> // Incluido para std::cout, etc.


// Añadido para seguir el estilo del ejemplo
using namespace std;



// --- Constantes ---
const int MAX_PREGUNTAS = 100;
const int NIVELES_TAXONOMIA = 6;
const string NIVELES[NIVELES_TAXONOMIA] = {
    "Recordar", "Entender", "Aplicar", "Analizar", "Evaluar", "Crear"
};
const int NUM_TIPOS_PREGUNTA = 5;
const string TIPOS_PREGUNTA[NUM_TIPOS_PREGUNTA] = {
    "Opción Múltiple", "Verdadero/Falso", "Respuesta Corta",
    "Composición/Ensayo", "Emparejamiento"
};


// --- Clase Pregunta 
class Pregunta {
private: // Atributos
    int id;
    string enunciado;
    string tipo;
    string nivelTaxonomico;
    int tiempoEstimado;
    string solucion;


public:
    // Constructor por defecto y parametrizado
    Pregunta();
    Pregunta(int id, string enunciado, string tipo, string nivel, int tiempo, string solucion);
    // Getters 
    int getId();
    string getEnunciado();
    string getTipo();
    string getNivelTaxonomico();
    int getTiempoEstimado();
    string getSolucion();

    // Setters
    void setId(int id);
    void setEnunciado(const string& enunciado); 
    void setTipo(const string& tipo);
    void setNivelTaxonomico(const string& nivel);
    void setTiempoEstimado(int tiempo);
    void setSolucion(const string& solucion);

    // Método para mostrar (siguiendo el estilo del ejemplo, sin const)
    void mostrar();
};

// --- Clase Profesor  
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

public:
    // Constructor
    Profesor();
    ~Profesor();
   


    // Funciones del menú 
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