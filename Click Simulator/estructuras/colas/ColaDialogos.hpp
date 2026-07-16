#pragma once
// =====================================================================
// ESTRUCTURA DE DATOS: COLA (QUEUE)
// ---------------------------------------------------------------------
// Se usa una cola para controlar el ORDEN en el que aparecen los
// elementos de un cuadro de diálogo: primero el texto de la pregunta,
// luego la opción izquierda, luego la opción derecha (y al cerrarse,
// el evento de cierre). Cada elemento se procesa en orden FIFO
// (el primero en encolarse es el primero en mostrarse/retirarse), que es
// exactamente el comportamiento que necesita una secuencia de aparición.
// =====================================================================
#include <deque>
#include <string>

struct EventoDialogo {
    std::string tipo;      // "TEXTO", "OPCION_IZQUIERDA", "OPCION_DERECHA", "CERRAR"
    std::string contenido; // texto a mostrar (vacío para "CERRAR")
};

class ColaDialogos {
public:
    void encolar(const EventoDialogo& evento) {
        datos.push_back(evento); // entra por el final
    }

    bool estaVacia() const { return datos.empty(); }

    EventoDialogo desencolar() {
        EventoDialogo e = datos.front(); // sale por el frente (FIFO)
        datos.pop_front();
        return e;
    }

    const EventoDialogo& frente() const { return datos.front(); }

    void vaciar() { datos.clear(); }

    int tamano() const { return static_cast<int>(datos.size()); }

private:
    std::deque<EventoDialogo> datos; // usado exclusivamente como cola (push_back / pop_front / front)
};
