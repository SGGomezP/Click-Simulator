#include "../../include/EscenaMenu.h"
#include "../../include/Utilidades.h"
#include <iostream>

EscenaMenu::EscenaMenu() : ancho(800.f), alto(600.f) {}

bool EscenaMenu::cargar(float anchoVentana, float altoVentana) {
    ancho = anchoVentana;
    alto = altoVentana;
    bool ok = true;

    if (!texturaFondo.loadFromFile("assets/images/menu/fondo_menu.png")) {
        std::cerr << "[EscenaMenu] Falta assets/images/menu/fondo_menu.png" << std::endl;
        ok = false;
    } else {
        spriteFondo.setTexture(texturaFondo, true);
        ajustarSpriteAVentana(spriteFondo, texturaFondo, ancho, alto);
        spriteFondo.setPosition(0.f, 0.f);
    }

    // El "gif" se arma con fotogramas assets/images/menu/gif_0001.png, 0002, ...
    auto rutasGif = generarRutasFrames("assets/images/menu", "gif_", ".png");
    if (rutasGif.empty()) {
        std::cerr << "[EscenaMenu] No se encontraron frames en assets/images/menu/ "
                     "(esperaba gif_0001.png, gif_0002.png, ...)" << std::endl;
        ok = false;
    } else {
        animGif.cargar(rutasGif, 0.1f, true); // en bucle
        const sf::Texture& primerFrame = animGif.getTexturaActual();
        spriteGif.setTexture(primerFrame, true);
        sf::Vector2u tam = primerFrame.getSize();
        spriteGif.setOrigin(tam.x / 2.f, tam.y / 2.f);
        // Centrado pero desplazado hacia la derecha
        spriteGif.setPosition(ancho * 0.68f, alto * 0.45f);
    }

    if (!texturaBoton.loadFromFile("assets/images/menu/boton_jugar.png")) {
        std::cerr << "[EscenaMenu] Falta assets/images/menu/boton_jugar.png" << std::endl;
        ok = false;
    } else {
        texturaBoton.setSmooth(false); // que se vea nítido el pixel art
        spriteBoton.setTexture(texturaBoton, true);
        spriteBoton.setScale(2.f, 2.f);
        sf::FloatRect b = spriteBoton.getLocalBounds();
        spriteBoton.setPosition(ancho * 0.06f, alto - (b.height * 2.f) - alto * 0.08f);
    }

    return ok;
}

void EscenaMenu::procesarEventos(const sf::Event& evento, sf::RenderWindow& ventana) {
    if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f click(static_cast<float>(evento.mouseButton.x), static_cast<float>(evento.mouseButton.y));
        if (spriteBoton.getGlobalBounds().contains(click)) {
            if (alJugar) alJugar();
        }
    }
    (void)ventana;
}

void EscenaMenu::actualizar(float dt) {
    animGif.actualizar(dt);
    spriteGif.setTexture(animGif.getTexturaActual(), true);
}

void EscenaMenu::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(spriteFondo);
    ventana.draw(spriteGif);
    ventana.draw(spriteBoton);
}

void EscenaMenu::establecerCallbackJugar(std::function<void()> callback) {
    alJugar = callback;
}
