#pragma once
// =====================================================================
// ESTRUCTURA DE DATOS: GRAFO DIRIGIDO (ADJACENCY LIST HASH TABLE)
// ---------------------------------------------------------------------
// El árbol de interacción del personaje se modela como un grafo dirigido.
// Cada NODO representa una pregunta o estado de diálogo, y cada ARISTA
// representa una opción de respuesta que conecta dicho nodo con el siguiente,
// o directamente con un final del juego.
//
// Al NO usar la STL, implementamos de manera manual:
// 1. Un Arreglo estático de opciones de tamaño fijo para las aristas salientes.
// 2. Una Tabla Hash personalizada con encadenamiento para mapear los IDs 
//    de los nodos con sus respectivas estructuras de datos en tiempo O(1).
// =====================================================================

namespace {
    // Funciones auxiliares inline para manipulación segura de strings estilo C
    inline void copiarCadena(char* destino, const char* origen, int limite) {
        int i = 0;
        if (origen) {
            while (i < limite && origen[i] != '\0') {
                destino[i] = origen[i];
                i++;
            }
        }
        destino[i] = '\0';
    }

    inline bool compararCadenas(const char* s1, const char* s2) {
        if (!s1 || !s2) return false;
        int i = 0;
        while (s1[i] != '\0' && s2[i] != '\0') {
            if (s1[i] != s2[i]) return false;
            i++;
        }
        return s1[i] == s2[i];
    }
}

struct OpcionInteraccion {
    char etiqueta[64];   // Texto del botón de diálogo
    char destinoId[32];  // ID del nodo de destino al que conduce esta opción

    OpcionInteraccion() {
        etiqueta[0] = '\0';
        destinoId[0] = '\0';
    }

    OpcionInteraccion(const char* et, const char* dest) {
        copiarCadena(etiqueta, et, 63);
        copiarCadena(destinoId, dest, 31);
    }
};

struct NodoInteraccion {
    char id[32];         // ID identificador único del nodo
    char texto[256];     // Texto o pregunta a mostrar en el cuadro
    
    // Al ser un juego conversacional, el límite de opciones por nodo es fijo
    static const int MAX_OPCIONES = 4; 
    OpcionInteraccion opciones[MAX_OPCIONES];
    int cantidadOpciones;

    NodoInteraccion() : cantidadOpciones(0) {
        id[0] = '\0';
        texto[0] = '\0';
    }

    NodoInteraccion(const char* nodeId, const char* nodeTexto) : cantidadOpciones(0) {
        copiarCadena(id, nodeId, 31);
        copiarCadena(texto, nodeTexto, 255);
    }

    void agregarOpcion(const OpcionInteraccion& op) {
        if (cantidadOpciones < MAX_OPCIONES) {
            opciones[cantidadOpciones] = op;
            cantidadOpciones++;
        }
    }
};

class GrafoInteracciones {
private:
    static const int CAPACIDAD = 31; // Tamaño primo óptimo para la dispersión del Hash

    // Nodo de colisiones para resolver mediante encadenamiento
    struct NodoHash {
        NodoInteraccion nodo;
        NodoHash* siguiente;
        NodoHash(const NodoInteraccion& n) : nodo(n), siguiente(nullptr) {}
    };

    NodoHash* tabla[CAPACIDAD];

    // Algoritmo Hash DJB2 para strings en C
    unsigned int calcularHash(const char* str) const {
        unsigned int hash = 5381;
        int c;
        while ((c = static_cast<unsigned char>(*str++))) {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return hash % CAPACIDAD;
    }

public:
    GrafoInteracciones() {
        for (int i = 0; i < CAPACIDAD; ++i) {
            tabla[i] = nullptr;
        }
    }

    // Destructor para liberar de forma limpia la memoria dinámica
    ~GrafoInteracciones() {
        vaciar();
    }

    void vaciar() {
        for (int i = 0; i < CAPACIDAD; ++i) {
            NodoHash* actual = tabla[i];
            while (actual != nullptr) {
                NodoHash* temporal = actual;
                actual = actual->siguiente;
                delete temporal; // Liberación explícita
            }
            tabla[i] = nullptr;
        }
    }

    // Agrega un nodo al Grafo (equivalente a insertar en la Tabla Hash)
    void agregarNodo(const char* id, const char* texto) {
        unsigned int indice = calcularHash(id);
        NodoHash* actual = tabla[indice];

        // Si ya existe, actualizamos su información para prevenir duplicados
        while (actual != nullptr) {
            if (compararCadenas(actual->nodo.id, id)) {
                copiarCadena(actual->nodo.texto, texto, 255);
                actual->nodo.cantidadOpciones = 0; // Reinicia las opciones/aristas
                return;
            }
            actual = actual->siguiente;
        }

        // Si es nuevo, se inserta al inicio de la lista del bucket (Complejidad O(1))
        NodoInteraccion nuevo(id, texto);
        NodoHash* nuevoNodoHash = new NodoHash(nuevo);
        nuevoNodoHash->siguiente = tabla[indice];
        tabla[indice] = nuevoNodoHash;
    }

    // Agrega una arista dirigida: nodoOrigen --etiqueta--> nodoDestino
    void agregarArista(const char* idOrigen, const char* etiqueta, const char* idDestino) {
        unsigned int indice = calcularHash(idOrigen);
        NodoHash* actual = tabla[indice];

        while (actual != nullptr) {
            if (compararCadenas(actual->nodo.id, idOrigen)) {
                OpcionInteraccion op(etiqueta, idDestino);
                actual->nodo.agregarOpcion(op);
                return;
            }
            actual = actual->siguiente;
        }
    }

    // Busca y retorna el nodo según su ID único
    const NodoInteraccion* obtenerNodo(const char* id) const {
        unsigned int indice = calcularHash(id);
        NodoHash* actual = tabla[indice];

        while (actual != nullptr) {
            if (compararCadenas(actual->nodo.id, id)) {
                return &(actual->nodo);
            }
            actual = actual->siguiente;
        }
        return nullptr; // No se encontró
    }

    // Verifica la existencia de un ID de diálogo en el grafo
    bool esNodoDeDialogo(const char* id) const {
        return obtenerNodo(id) != nullptr;
    }
};