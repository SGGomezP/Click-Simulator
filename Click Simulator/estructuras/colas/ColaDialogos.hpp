#pragma once
// =====================================================================
// ESTRUCTURA DE DATOS: COLA DINÁMICA (LINKED QUEUE)
// ---------------------------------------------------------------------
// Se usa una estructura de tipo Cola para asegurar un procesamiento FIFO
// (First-In, First-Out) de los eventos de la interfaz de diálogo.
//
// Al NO usar la STL, implementamos de manera manual los nodos enlazados
// y gestionamos la memoria dinámica directamente, asegurando operaciones
// de inserción y extracción en tiempo constante O(1).
// =====================================================================
#include <iostream> // Para depuración en consola si fuera necesario

// Reemplazo de los strings de tipo por un Enumerado (Mucho más eficiente)
enum class TipoEvento {
    TEXTO,
    OPCION_IZQUIERDA,
    OPCION_DERECHA,
    CERRAR
};

struct EventoDialogo {
    TipoEvento tipo;
    char contenido[256]; // Reemplazo de std::string por C-string estático

    // Constructor por defecto
    EventoDialogo() : tipo(TipoEvento::CERRAR) {
        contenido[0] = '\0';
    }

    // Constructor para inicializar de forma segura y evitar desbordamientos
    EventoDialogo(TipoEvento t, const char* texto) : tipo(t) {
        int i = 0;
        if (texto) {
            // Copia segura de caracteres evitando desbordamiento del buffer
            while (i < 255 && texto[i] != '\0') {
                contenido[i] = texto[i];
                i++;
            }
        }
        contenido[i] = '\0'; // Carácter nulo de terminación
    }
};

class ColaDialogos {
private:
    // Estructura interna de los nodos de la lista enlazada
    struct Nodo {
        EventoDialogo evento;
        Nodo* siguiente;
        Nodo(const EventoDialogo& ev) : evento(ev), siguiente(nullptr) {}
    };

    Nodo* inicio;  // Puntero al primer nodo (Frente / Salida)
    Nodo* fin;     // Puntero al último nodo (Final / Entrada)
    int cantidad;  // Contador de elementos en la cola

public:
    ColaDialogos() : inicio(nullptr), fin(nullptr), cantidad(0) {}

    // Destructor: Indispensable para liberar memoria dinámica al destruir la cola
    ~ColaDialogos() {
        vaciar();
    }

    // Inserta un evento al final de la cola (FIFO)
    void encolar(const EventoDialogo& evento) {
        Nodo* nuevo = new Nodo(evento);
        if (estaVacia()) {
            inicio = nuevo;
            fin = nuevo;
        } else {
            fin->siguiente = nuevo;
            fin = nuevo;
        }
        cantidad++;
    }

    // Verifica si la cola carece de elementos
    bool estaVacia() const { 
        return inicio == nullptr; 
    }

    // Retira y devuelve el elemento que está al frente de la cola
    EventoDialogo desencolar() {
        if (estaVacia()) {
            // Retorno seguro en caso de error/vacío
            return EventoDialogo(TipoEvento::CERRAR, "");
        }

        Nodo* temp = inicio;
        EventoDialogo eventoRetorno = temp->evento;

        inicio = inicio->siguiente;
        delete temp; // Liberación de memoria dinámica del nodo

        // Si la cola queda vacía, el puntero del final también debe ser nulo
        if (inicio == nullptr) {
            fin = nullptr;
        }

        cantidad--;
        return eventoRetorno;
    }

    // Devuelve el elemento al frente sin retirarlo
    const EventoDialogo& frente() const { 
        if (estaVacia()) {
            static EventoDialogo vacio(TipoEvento::CERRAR, "");
            return vacio;
        }
        return inicio->evento; 
    }

    // Elimina todos los elementos y libera la memoria acumulada
    void vaciar() { 
        while (!estaVacia()) {
            desencolar();
        }
    }

    // Retorna la cantidad de elementos en la cola
    int tamano() const { 
        return cantidad; 
    }
};