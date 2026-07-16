#pragma once
#include <SFML/Graphics.hpp>
#include "EstadoJuego.h"
#include "EscenaIntro.h"
#include "EscenaMenu.h"
#include "EscenaJuego.h"
#include "ManejadorAudio.h"
#include "../estructuras/pilas/PilaEscenas.hpp"

// Orquestador principal: crea la ventana, mantiene el estado actual
// (INTRO / MENU / JUEGO) y delega eventos/actualizacion/dibujado a la
// escena activa.
class Game {
private:
    sf::RenderWindow window;
    EstadoJuego estadoActual;
    PilaEscenas historialEscenas; // PILA: historial de cambios de escena

    EscenaIntro escenaIntro;
    EscenaMenu escenaMenu;
    EscenaJuego escenaJuego;
    ManejadorAudio audio;

    sf::Clock reloj;

    void initWindow();
    void cambiarEscena(EstadoJuego nuevo);
    std::string nombreEscena(EstadoJuego estado) const;

public:
    Game();
    ~Game();

    void run();
    void processEvents();
    void update(float dt);
    void render();
};
