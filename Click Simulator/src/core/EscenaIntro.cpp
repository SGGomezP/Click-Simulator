#include "../../include/EscenaIntro.h"
#include "../../include/Utilidades.h"
#include <iostream>

EscenaIntro::EscenaIntro()
    : ancho(800.f), alto(600.f), enDestello(false), tiempoDestello(0.f), yaAviso(false) {}

bool EscenaIntro::cargar(float anchoVentana, float altoVentana) {
    ancho = anchoVentana;
    alto = altoVentana;

    // El "video" se arma con fotogramas assets/images/intro/frame_0001.png, 0002, ...
    auto rutas = generarRutasFrames("assets/images/intro", "frame_", ".png");
    if (rutas.empty()) {
        std::cerr << "[EscenaIntro] No se encontraron frames en assets/images/intro/ "
                     "(esperaba frame_0001.png, frame_0002.png, ...)" << std::endl;
        return false;
    }

    if (!animVideo.cargar(rutas, 0.05f, false)) return false; // 20 fps aprox, no se repite

    overlayBlanco.setSize(sf::Vector2f(ancho, alto));
    overlayBlanco.setFillColor(sf::Color::White);
    return true;
}

void EscenaIntro::procesarEventos(const sf::Event& /*evento*/) {
    // Sin interacción durante el video por ahora.
}

void EscenaIntro::actualizar(float dt) {
    if (!enDestello) {
        animVideo.actualizar(dt);
        if (animVideo.haTerminado()) {
            enDestello = true;
            tiempoDestello = 0.f;
        }
    } else {
        tiempoDestello += dt;
        if (tiempoDestello >= DURACION_DESTELLO && !yaAviso) {
            yaAviso = true;
            if (alTerminar) alTerminar();
        }
    }
}

void EscenaIntro::dibujar(sf::RenderWindow& ventana) {
    if (!enDestello) {
        const sf::Texture& textura = animVideo.getTexturaActual();
        spriteVideo.setTexture(textura, true);
        ajustarSpriteAVentana(spriteVideo, textura, ancho, alto);
        spriteVideo.setPosition(0.f, 0.f);
        ventana.draw(spriteVideo);
    } else {
        // Todo blanco: la explosión final del video "revienta" en blanco puro
        ventana.draw(overlayBlanco);
    }
}

void EscenaIntro::establecerCallbackFin(std::function<void()> callback) {
    alTerminar = callback;
}

void EscenaIntro::reiniciar() {
    animVideo.reiniciar();
    enDestello = false;
    tiempoDestello = 0.f;
    yaAviso = false;
}
