#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <functional>

// Pantalla de apertura del juego (ya NO es un video de frames):
// 1) Arranca en negro, sin sonido.
// 2) La pantalla se va aclarando gradualmente hasta quedar blanca.
// 3) Al quedar totalmente blanca, suena un audio.
// 4) Cuando ese audio termina, se avisa a Game para pasar al Menú
//    (que es quien arranca su propia música ahí).
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
    enum class Fase { NEGRO, DESVANECIENDO, BLANCO_CON_SONIDO };

    float ancho, alto;
    Fase fase;
    float tiempoEnFase;

    static constexpr float DURACION_NEGRO = 0.6f;        // pantalla negra fija, sin sonido
    static constexpr float DURACION_DESVANECIDO = 1.8f;  // negro -> blanco gradual
    static constexpr float ESPERA_MAXIMA_SIN_AUDIO = 2.0f; // salvavidas si falta el archivo de audio

    sf::RectangleShape overlay;

    sf::SoundBuffer bufferSonido;
    sf::Sound sonido;
    bool sonidoCargado;
    bool sonidoReproducido;

    bool yaAviso;
    std::function<void()> alTerminar;
};
