#pragma once
// =====================================================================
// ESTRUCTURA DE DATOS: PILA (STACK)
// ---------------------------------------------------------------------
// Se usa una pila para registrar el cambio de escenas del juego
// (INTRO -> MENU -> JUEGO). Cada vez que el juego cambia de pantalla se
// apila (push) el nombre de la nueva escena; el tope (top) siempre indica
// la escena activa en ese momento. Se restringe el uso de un
// std::vector únicamente a operaciones de pila (apilar/desapilar/tope)
// para respetar el comportamiento LIFO (el último en entrar es el primero
// en salir), dejando además un historial de navegación por si en el
// futuro se agrega la posibilidad de "volver atrás".
// =====================================================================
#include <vector>
#include <string>

class PilaEscenas {
public:
    void apilar(const std::string& escena) {
        datos.push_back(escena);
    }

    void desapilar() {
        if (!estaVacia()) datos.pop_back();
    }

    std::string tope() const {
        if (estaVacia()) return "";
        return datos.back();
    }

    bool estaVacia() const { return datos.empty(); }

    int tamano() const { return static_cast<int>(datos.size()); }

    // Historial completo, de la escena más antigua a la más reciente (solo lectura/depuración)
    const std::vector<std::string>& historial() const { return datos; }

private:
    std::vector<std::string> datos; // usado exclusivamente como pila (push_back / pop_back / back)
};
