#pragma once
// =====================================================================
// ESTRUCTURA DE DATOS: TABLA HASH CON ENCADENAMIENTO (HASH TABLE)
// ---------------------------------------------------------------------
// Se usa una Tabla Hash propia para almacenar el "expediente" o progreso 
// del juego en formato Clave-Valor (ej: "clicks" -> "42", "final" -> "true").
//
// Al NO usar la STL, implementamos:
// 1. Una función hash propia (Algoritmo DJB2).
// 2. Direccionamiento cerrado mediante nodos enlazados para colisiones.
// 3. Manejo de archivos con C-Style I/O (fopen/fprintf) libre de streams.
// =====================================================================
#include <cstdio>  // Para operaciones nativas de archivos (fopen, fprintf, fgets)
#include <iostream>

class TablaHashGuardado {
private:
    static const int CAPACIDAD = 31; // Tamaño de la tabla (un número primo óptimo)

    // Estructura para cada par Clave-Valor en caso de colisiones (Lista Enlazada)
    struct Nodo {
        char clave[64];    // Buffer estático seguro para la clave
        char valor[128];   // Buffer estático seguro para el valor
        Nodo* siguiente;

        Nodo(const char* k, const char* v) : siguiente(nullptr) {
            copiarCadena(clave, k, 63);
            copiarCadena(valor, v, 127);
        }

    private:
        // Función auxiliar estática para copiar strings estilo C sin desbordamiento
        static void copiarCadena(char* destino, const char* origen, int max_limite) {
            int i = 0;
            if (origen) {
                while (i < max_limite && origen[i] != '\0') {
                    destino[i] = origen[i];
                    i++;
                }
            }
            destino[i] = '\0';
        }
    };

    Nodo* tabla[CAPACIDAD]; // Arreglo de punteros a Nodos (Buckets)

    // Algoritmo de Hashing DJB2: Excelente distribución y poquísimas colisiones
    unsigned int calcularHash(const char* str) const {
        unsigned int hash = 5381;
        int c;
        while ((c = static_cast<unsigned char>(*str++))) {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return hash % CAPACIDAD;
    }

    // Funciones auxiliares para trabajar con cadenas de estilo C sin <cstring>
    bool compararCadenas(const char* str1, const char* str2) const {
        if (!str1 || !str2) return false;
        int i = 0;
        while (str1[i] != '\0' && str2[i] != '\0') {
            if (str1[i] != str2[i]) return false;
            i++;
        }
        return str1[i] == str2[i];
    }

    void copiarCadena(char* destino, const char* origen, int max_limite) const {
        int i = 0;
        if (origen) {
            while (i < max_limite && origen[i] != '\0') {
                destino[i] = origen[i];
                i++;
            }
        }
        destino[i] = '\0';
    }

public:
    TablaHashGuardado() {
        for (int i = 0; i < CAPACIDAD; ++i) {
            tabla[i] = nullptr;
        }
    }

    // Destructor indispensable para evitar fugas de memoria
    ~TablaHashGuardado() {
        limpiar();
    }

    // Inserta o actualiza un elemento en la Tabla Hash
    void set(const char* clave, const char* valor) {
        unsigned int indice = calcularHash(clave);
        Nodo* actual = tabla[indice];

        // Buscar si la clave ya existe para actualizar su valor
        while (actual != nullptr) {
            if (compararCadenas(actual->clave, clave)) {
                copiarCadena(actual->valor, valor, 127);
                return; // Actualizado exitosamente
            }
            actual = actual->siguiente;
        }

        // Si no existe, insertar al inicio de la lista (Inserción O(1))
        Nodo* nuevo = new Nodo(clave, valor);
        nuevo->siguiente = tabla[indice];
        tabla[indice] = nuevo;
    }

    // Obtiene el valor asociado a una clave, o devuelve el valor por defecto
    const char* get(const char* clave, const char* porDefecto = "") const {
        unsigned int indice = calcularHash(clave);
        Nodo* actual = tabla[indice];

        while (actual != nullptr) {
            if (compararCadenas(actual->clave, clave)) {
                return actual->valor;
            }
            actual = actual->siguiente;
        }
        return porDefecto;
    }

    // Verifica si la clave existe en la tabla
    bool existe(const char* clave) const {
        unsigned int indice = calcularHash(clave);
        Nodo* actual = tabla[indice];

        while (actual != nullptr) {
            if (compararCadenas(actual->clave, clave)) {
                return true;
            }
            actual = actual->siguiente;
        }
        return false;
    }

    // Libera de forma segura la memoria de todos los nodos creados
    void limpiar() {
        for (int i = 0; i < CAPACIDAD; ++i) {
            Nodo* actual = tabla[i];
            while (actual != nullptr) {
                Nodo* temporal = actual;
                actual = actual->siguiente;
                delete temporal;
            }
            tabla[i] = nullptr;
        }
    }

    // Guarda el progreso usando punteros a archivo físicos nativos
    bool guardarEnArchivo(const char* ruta) const {
        // Usamos fopen nativo de C para omitir los streams de la STL
        FILE* archivo = std::fopen(ruta, "w");
        if (!archivo) return false;

        for (int i = 0; i < CAPACIDAD; ++i) {
            Nodo* actual = tabla[i];
            while (actual != nullptr) {
                std::fprintf(archivo, "%s=%s\n", actual->clave, actual->valor);
                actual = actual->siguiente;
            }
        }

        std::fclose(archivo);
        return true;
    }

    // Carga el progreso leyendo línea a línea sin std::getline
    bool cargarDesdeArchivo(const char* ruta) {
        FILE* archivo = std::fopen(ruta, "r");
        if (!archivo) return false;

        limpiar(); // Reiniciar el estado antes de cargar

        char linea[256];
        // fgets lee de forma segura línea por línea evitando desbordamientos
        while (std::fgets(linea, sizeof(linea), archivo)) {
            // Remover saltos de línea al final ('\n' o '\r')
            int len = 0;
            while (linea[len] != '\0') len++;
            if (len > 0 && (linea[len - 1] == '\n' || linea[len - 1] == '\r')) {
                linea[len - 1] = '\0';
                if (len > 1 && (linea[len - 2] == '\r')) {
                    linea[len - 2] = '\0';
                }
            }

            // Buscar la posición del carácter '=' divisor
            int posIgual = -1;
            for (int i = 0; linea[i] != '\0'; ++i) {
                if (linea[i] == '=') {
                    posIgual = i;
                    break;
                }
            }

            // Si la línea es válida "clave=valor", la procesamos
            if (posIgual != -1) {
                linea[posIgual] = '\0'; // Dividimos temporalmente la string
                const char* k = linea;
                const char* v = &linea[posIgual + 1];
                set(k, v); // Se inserta directo en la Tabla Hash
            }
        }

        std::fclose(archivo);
        return true;
    }
};