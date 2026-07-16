#pragma once
// =====================================================================
// ESTRUCTURA DE DATOS: PILA ESTÁTICA SECUENCIAL (STACK)
// ---------------------------------------------------------------------
// Registra las pantallas activas del juego para respetar el comportamiento LIFO[cite: 85, 88].
// Al NO usar la STL, implementamos:
// 1. Almacenamiento en buffer contiguo de memoria estática (Fast-path).
// 2. Acceso directo por índice y control seguro de desbordamientos del límite de pila.
// =====================================================================

class PilaEscenas {
private:
    static const int MAX_ESCENAS = 32;  // Límite de profundidad física de la pila
    char datos[MAX_ESCENAS][64];        // Buffer estático de nombres de escenas
    int cantidad;

    static void copiarCadena(char* destino, const char* origen, int limite) {
        int i = 0;
        if (origen) {
            while (i < limite && origen[i] != '\0') {
                destino[i] = origen[i];
                i++;
            }
        }
        destino[i] = '\0';
    }

public:
    PilaEscenas() : cantidad(0) {}

    // Apila un nuevo estado de escena (Push)
    void apilar(const char* escena) {
        if (cantidad < MAX_ESCENAS) {
            copiarCadena(datos[cantidad], escena, 63);
            cantidad++;
        }
    }

    // Desapila la escena activa (Pop)
    void desapilar() {
        if (!estaVacia()) {
            cantidad--;
        }
    }

    // Retorna la escena en la cima (Top / Peek)
    const char* tope() const {
        if (estaVacia()) return "";
        return datos[cantidad - 1];
    }

    bool estaVacia() const {
        return cantidad == 0;
    }

    int tamano() const {
        return cantidad;
    }

    // Sustitución del historial original: Acceso secuencial seguro de lectura
    const char* obtenerAt(int indice) const {
        if (indice >= 0 && indice < cantidad) {
            return datos[indice];
        }
        return "";
    }
};