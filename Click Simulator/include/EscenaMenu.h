#pragma once
#include <SFML/Graphics.hpp>
#include "../estructuras/arrays/ArrayAnimacion.hpp"
#include <functional>

class EscenaMenu {
public:
    EscenaMenu();

    bool cargar(float anchoVentana, float altoVentana);
    void procesarEventos(const sf::Event& evento, sf::RenderWindow& ventana);
    void actualizar(float dt);
    void dibujar(sf::RenderWindow& ventana);

    void establecerCallbackJugar(std::function<void()> callback);

private:
    float ancho, alto;

    sf::Texture texturaFondo;
    sf::Sprite spriteFondo;

    ArrayAnimacion animGif; // el "gif" del menú, recortado de un sprite sheet, en bucle
    sf::Sprite spriteGif;

    sf::Texture texturaBoton;
    sf::Sprite spriteBoton;

    std::function<void()> alJugar;
};
