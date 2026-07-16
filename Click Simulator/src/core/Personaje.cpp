#include "../../include/Personaje.h"

Personaje::Personaje()
    : ajusteFino(1.f), tiempoEfectoClick(0.f), finalActivo(TipoFinal::NINGUNO),
      deslizandoIzquierda(false), deslizandoDerecha(false), salioDePantalla(false) {}

bool Personaje::cargarIdle(const std::vector<std::string>& rutas) {
    return animIdle.cargar(rutas, 0.18f, true); // en bucle
}

bool Personaje::cargarFinalBeso(const std::vector<std::string>& rutas) {
    return animBeso.cargar(rutas, 0.15f, false); // se queda en el ultimo frame
}

bool Personaje::cargarFinalSaludo(const std::vector<std::string>& rutas) {
    return animSaludo.cargar(rutas, 0.15f, false);
}

bool Personaje::cargarFinalPose(const std::vector<std::string>& rutas) {
    return animPose.cargar(rutas, 0.12f, false);
}

bool Personaje::cargarFinalPatineta(const std::vector<std::string>& rutas) {
    return animPatineta.cargar(rutas, 0.15f, true); // pedalea mientras se desliza
}

void Personaje::establecerPosicion(float x, float y) {
    posicionBase = sf::Vector2f(x, y);
    sprite.setPosition(posicionBase);
}

void Personaje::actualizar(float dt, float anchoVentana, float altoVentana) {
    // Animación activa según el estado actual
    switch (finalActivo) {
        case TipoFinal::NINGUNO:   animIdle.actualizar(dt);     break;
        case TipoFinal::BESO:      animBeso.actualizar(dt);     break;
        case TipoFinal::SALUDO:    animSaludo.actualizar(dt);   break;
        case TipoFinal::POSE:      animPose.actualizar(dt);     break;
        case TipoFinal::PATINETA:  animPatineta.actualizar(dt); break;
    }

    // Efecto de click: se achica un poco y vuelve a la normalidad
    float escalaClick = 1.f;
    if (tiempoEfectoClick > 0.f) {
        tiempoEfectoClick -= dt;
        float progreso = tiempoEfectoClick / DURACION_EFECTO_CLICK; // 1 -> 0
        escalaClick = 1.f - 0.12f * progreso; // se achica hasta un 12%
    }

    const sf::Texture* textura = nullptr;
    switch (finalActivo) {
        case TipoFinal::NINGUNO:  textura = &animIdle.getTexturaActual();     break;
        case TipoFinal::BESO:     textura = &animBeso.getTexturaActual();     break;
        case TipoFinal::SALUDO:   textura = &animSaludo.getTexturaActual();  break;
        case TipoFinal::POSE:     textura = &animPose.getTexturaActual();    break;
        case TipoFinal::PATINETA: textura = &animPatineta.getTexturaActual();break;
    }
    if (textura) {
        sprite.setTexture(*textura, true);
        sf::Vector2u tam = textura->getSize();
        sprite.setOrigin(tam.x / 2.f, tam.y / 2.f);

        // Escala automática: sin importar cuántos píxeles mida el sprite
        // original, el personaje siempre ocupa FRACCION_ALTO_VENTANA del
        // alto de la ventana, para que el fondo y la UI queden visibles.
        float alturaObjetivo = altoVentana * FRACCION_ALTO_VENTANA;
        float escalaAuto = (tam.y > 0) ? (alturaObjetivo / static_cast<float>(tam.y)) : 1.f;

        sprite.setScale(escalaAuto * ajusteFino * escalaClick, escalaAuto * ajusteFino * escalaClick);
    }

    // Deslizamiento de salida para los finales 2 (saludo, izquierda) y 4 (patineta, derecha)
    if (deslizandoIzquierda) {
        sprite.move(-VELOCIDAD_DESLIZAMIENTO * dt, 0.f);
        if (sprite.getPosition().x + sprite.getGlobalBounds().width < 0.f) {
            salioDePantalla = true;
            deslizandoIzquierda = false;
        }
    } else if (deslizandoDerecha) {
        sprite.move(VELOCIDAD_DESLIZAMIENTO * dt, 0.f);
        if (sprite.getPosition().x > anchoVentana) {
            salioDePantalla = true;
            deslizandoDerecha = false;
        }
    } else {
        sprite.setPosition(posicionBase.x, posicionBase.y);
    }
}

void Personaje::dibujar(sf::RenderWindow& ventana) {
    if (salioDePantalla) return; // ya no se dibuja, quedó fuera
    ventana.draw(sprite);
}

bool Personaje::contienePunto(sf::Vector2f punto) const {
    return sprite.getGlobalBounds().contains(punto);
}

void Personaje::alHacerClick() {
    tiempoEfectoClick = DURACION_EFECTO_CLICK;
}

void Personaje::iniciarFinal(TipoFinal tipo) {
    finalActivo = tipo;
    salioDePantalla = false;
    deslizandoIzquierda = false;
    deslizandoDerecha = false;

    switch (tipo) {
        case TipoFinal::BESO:     animBeso.reiniciar();     break;
        case TipoFinal::SALUDO:   animSaludo.reiniciar();   deslizandoIzquierda = true; break;
        case TipoFinal::POSE:     animPose.reiniciar();     break;
        case TipoFinal::PATINETA: animPatineta.reiniciar(); deslizandoDerecha = true;   break;
        default: break;
    }
}
