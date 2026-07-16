#include "../../include/Game.h"
#include <iostream>

Game::Game() {
    initWindow();
    initBackground();
}

Game::~Game() {
    // Constructor vacío por ahora
}

void Game::initWindow() {
    // Creamos una ventana de 800x600 píxeles
    window.create(sf::VideoMode(800, 600), "Click Simulator", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60); // Limitamos a 60 FPS
}

void Game::initBackground() {
    // Intentamos cargar la imagen desde la carpeta assets
    if (!backgroundTexture.loadFromFile("assets/images/fondo.png")) {
        std::cerr << "Error: No se pudo cargar assets/images/fondo.png. Intentando con .jpg..." << std::endl;
        if (!backgroundTexture.loadFromFile("assets/images/fondo.jpg")) {
            std::cerr << "Error fatal: No se encontro ninguna imagen de fondo." << std::endl;
        }
    }
    
    backgroundSprite.setTexture(backgroundTexture);

    // Ajustar la imagen al tamaño de la ventana (800x600)
    sf::Vector2u size = backgroundTexture.getSize();
    backgroundSprite.setScale(800.f / size.x, 600.f / size.y);
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        // Si el usuario da clic en la 'X', se cierra la ventana
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

void Game::update() {
    // Aquí irá el contador de clics y animaciones más adelante
}

void Game::render() {
    window.clear(); // Limpia la pantalla
    window.draw(backgroundSprite); // Dibuja el fondo
    window.display(); // Muestra lo dibujado en la ventana
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        update();
        render();
    }
}