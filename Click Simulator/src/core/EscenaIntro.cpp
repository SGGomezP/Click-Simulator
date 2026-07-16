#include "../../include/EscenaIntro.h"
#include <iostream>

EscenaIntro::EscenaIntro()
    : ancho(800.f), alto(600.f), fase(Fase::NEGRO), tiempoEnFase(0.f),
      sonidoCargado(false), sonidoReproducido(false), yaAviso(false) {}

bool EscenaIntro::cargar(float anchoVentana, float altoVentana) {
    ancho = anchoVentana;
    alto = altoVentana;

    overlay.setSize(sf::Vector2f(ancho, alto));
    overlay.setFillColor(sf::Color::Black);

    sonidoCargado = bufferSonido.loadFromFile("assets/sonidos/apertura.ogg");
    if (sonidoCargado) {
        sonido.setBuffer(bufferSonido);
    } else {
        std::cerr << "[EscenaIntro] Falta assets/sonidos/apertura.ogg "
                     "(el audio que suena cuando la pantalla queda totalmente blanca)." << std::endl;
    }

    // Esta escena no depende de imagenes externas, siempre se puede mostrar.
    return true;
}

void EscenaIntro::procesarEventos(const sf::Event& /*evento*/) {
    // Sin interaccion durante la apertura.
}

void EscenaIntro::actualizar(float dt) {
    if (yaAviso) return;
    tiempoEnFase += dt;

    switch (fase) {
        case Fase::NEGRO:
            if (tiempoEnFase >= DURACION_NEGRO) {
                fase = Fase::DESVANECIENDO;
                tiempoEnFase = 0.f;
            }
            break;

        case Fase::DESVANECIENDO:
            if (tiempoEnFase >= DURACION_DESVANECIDO) {
                fase = Fase::BLANCO_CON_SONIDO;
                tiempoEnFase = 0.f;
                if (sonidoCargado) {
                    sonido.play();
                    sonidoReproducido = true;
                }
            }
            break;

        case Fase::BLANCO_CON_SONIDO:
            if (sonidoReproducido) {
                // Se espera a que el audio termine de reproducirse por completo.
                if (sonido.getStatus() != sf::Sound::Playing) {
                    yaAviso = true;
                    if (alTerminar) alTerminar();
                }
            } else if (tiempoEnFase >= ESPERA_MAXIMA_SIN_AUDIO) {
                // No hay audio cargado (falta el archivo): no se queda trabado
                // en blanco para siempre, pasa al menú tras una breve espera.
                yaAviso = true;
                if (alTerminar) alTerminar();
            }
            break;
    }
}

void EscenaIntro::dibujar(sf::RenderWindow& ventana) {
    sf::Color color = sf::Color::Black;

    switch (fase) {
        case Fase::NEGRO:
            color = sf::Color::Black;
            break;
        case Fase::DESVANECIENDO: {
            float t = (DURACION_DESVANECIDO > 0.f) ? (tiempoEnFase / DURACION_DESVANECIDO) : 1.f;
            if (t > 1.f) t = 1.f;
            sf::Uint8 v = static_cast<sf::Uint8>(255.f * t); // negro (0) -> blanco (255)
            color = sf::Color(v, v, v);
            break;
        }
        case Fase::BLANCO_CON_SONIDO:
            color = sf::Color::White;
            break;
    }

    overlay.setFillColor(color);
    ventana.draw(overlay);
}

void EscenaIntro::establecerCallbackFin(std::function<void()> callback) {
    alTerminar = callback;
}

void EscenaIntro::reiniciar() {
    fase = Fase::NEGRO;
    tiempoEnFase = 0.f;
    sonido.stop();
    sonidoReproducido = false;
    yaAviso = false;
}
