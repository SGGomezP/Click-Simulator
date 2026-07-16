#pragma once
// =====================================================================
// ESTRUCTURA DE DATOS: GRAFO (GRAPH)
// ---------------------------------------------------------------------
// El árbol de interacción del personaje se modela como un grafo dirigido:
// cada NODO representa una pregunta/estado de diálogo, y cada ARISTA
// (una opción de respuesta) conecta ese nodo con el siguiente nodo, o
// directamente con un "final" (F1..F4). Esto permite manejar
// interacciones ramificadas (una misma pregunta puede llevar a varios
// caminos distintos) de forma ordenada y fácil de ampliar: para agregar
// una pregunta nueva solo hay que agregar un nodo y sus aristas.
//
// Representación interna: lista de adyacencia (unordered_map de id -> nodo,
// y cada nodo guarda su propia lista de aristas salientes).
// =====================================================================
#include <string>
#include <vector>
#include <unordered_map>

struct OpcionInteraccion {
    std::string etiqueta;  // Texto del botón, ej: "Coquetear"
    std::string destinoId; // Id del nodo o del final al que lleva esta arista
};

struct NodoInteraccion {
    std::string id;
    std::string texto;                      // Pregunta que se muestra, ej: "Hola nena"
    std::vector<OpcionInteraccion> opciones; // aristas salientes (máx. 2 en este juego)
};

class GrafoInteracciones {
public:
    void agregarNodo(const std::string& id, const std::string& texto) {
        NodoInteraccion nodo;
        nodo.id = id;
        nodo.texto = texto;
        nodos[id] = nodo;
    }

    // Agrega una arista dirigida: nodoOrigen --etiqueta--> nodoDestino (o final)
    void agregarArista(const std::string& idOrigen, const std::string& etiqueta, const std::string& idDestino) {
        auto it = nodos.find(idOrigen);
        if (it != nodos.end()) {
            OpcionInteraccion op;
            op.etiqueta = etiqueta;
            op.destinoId = idDestino;
            it->second.opciones.push_back(op);
        }
    }

    const NodoInteraccion* obtenerNodo(const std::string& id) const {
        auto it = nodos.find(id);
        if (it == nodos.end()) return nullptr;
        return &it->second;
    }

    bool esNodoDeDialogo(const std::string& id) const {
        return nodos.find(id) != nodos.end();
    }

private:
    std::unordered_map<std::string, NodoInteraccion> nodos; // lista de adyacencia del grafo
};
