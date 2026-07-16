#pragma once
#include <SFML/Graphics.hpp>
#include "../estructuras/arrays/ArrayAnimacion.hpp"
#include <functional>

// Reproduce el "video" de intro (secuencia de imágenes) y, al terminar,
// hace un destello blanco antes de avisar a Game que pase al Menú.
class EscenaIntro {
public:
    EscenaIntro();

    bool cargar(float anchoVentana, float altoVentana);
    void procesarEventos(const sf::Event& evento);
    void actualizar(float dt);
    void dibujar(sf::RenderWindow& ventana);

    void establecerCallbackFin(std::function<void()> callback);
    void reiniciar();

private:
    ArrayAnimacion animVideo;
    sf::Sprite spriteVideo;
    float ancho, alto;

    bool enDestello;
    float tiempoDestello;
    static constexpr float DURACION_DESTELLO = 0.7f;

    bool yaAviso;
    std::function<void()> alTerminar;

    sf::RectangleShape overlayBlanco;
};
