#include "../../include/Game.h"
#include <iostream>

Game::Game() : estadoActual(EstadoJuego::INTRO) {
    initWindow();

    if (!escenaIntro.cargar(800.f, 600.f)) {
        std::cerr << "[Game] La escena de intro no cargo correctamente (ver mensajes arriba)." << std::endl;
    }
    if (!escenaMenu.cargar(800.f, 600.f)) {
        std::cerr << "[Game] La escena de menu no cargo correctamente (ver mensajes arriba)." << std::endl;
    }
    if (!escenaJuego.cargar(800.f, 600.f)) {
        std::cerr << "[Game] La escena de juego no cargo correctamente (ver mensajes arriba)." << std::endl;
    }
    escenaJuego.establecerAudio(&audio);

    audio.cargarMusicaMenu("assets/sonidos/musica_menu.ogg");
    audio.cargarMusicaJuego("assets/sonidos/musica_juego.ogg");
    audio.cargarSonidoDespedida("assets/sonidos/despedida.ogg");

    std::vector<std::string> rutasVoces;
    for (int i = 1; i <= 10; ++i) {
        rutasVoces.push_back("assets/sonidos/voces/voz_click_" + std::string(i < 10 ? "0" : "") + std::to_string(i) + ".ogg");
    }
    audio.cargarVocesClick(rutasVoces);

    historialEscenas.apilar(nombreEscena(estadoActual));

    escenaIntro.establecerCallbackFin([this]() {
        cambiarEscena(EstadoJuego::MENU);
        audio.reproducirMusicaMenu();
    });

    escenaMenu.establecerCallbackJugar([this]() {
        cambiarEscena(EstadoJuego::JUEGO);
        audio.reproducirMusicaJuego();
    });
}

Game::~Game() {}

void Game::initWindow() {
    // Se mantiene la resolucion original del proyecto (800x600).
    window.create(sf::VideoMode(800, 600), "Click Simulator", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
}

std::string Game::nombreEscena(EstadoJuego estado) const {
    switch (estado) {
        case EstadoJuego::INTRO: return "INTRO";
        case EstadoJuego::MENU:  return "MENU";
        case EstadoJuego::JUEGO: return "JUEGO";
    }
    return "";
}

void Game::cambiarEscena(EstadoJuego nuevo) {
    estadoActual = nuevo;
    historialEscenas.apilar(nombreEscena(nuevo)); // PILA: se apila la nueva escena activa
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            if (estadoActual == EstadoJuego::JUEGO) {
                escenaJuego.alCerrarVentana(); // guarda o borra el progreso segun corresponda
            }
            window.close();
            continue;
        }

        switch (estadoActual) {
            case EstadoJuego::INTRO: escenaIntro.procesarEventos(event); break;
            case EstadoJuego::MENU:  escenaMenu.procesarEventos(event, window); break;
            case EstadoJuego::JUEGO: escenaJuego.procesarEventos(event); break;
        }
    }
}

void Game::update(float dt) {
    switch (estadoActual) {
        case EstadoJuego::INTRO: escenaIntro.actualizar(dt); break;
        case EstadoJuego::MENU:  escenaMenu.actualizar(dt); break;
        case EstadoJuego::JUEGO: escenaJuego.actualizar(dt); break;
    }
}

void Game::render() {
    window.clear();
    switch (estadoActual) {
        case EstadoJuego::INTRO: escenaIntro.dibujar(window); break;
        case EstadoJuego::MENU:  escenaMenu.dibujar(window); break;
        case EstadoJuego::JUEGO: escenaJuego.dibujar(window); break;
    }
    window.display();
}

void Game::run() {
    while (window.isOpen()) {
        float dt = reloj.restart().asSeconds();
        processEvents();
        update(dt);
        render();
    }
}
