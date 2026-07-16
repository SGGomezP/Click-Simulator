#pragma once
// =====================================================================
// ESTRUCTURA DE DATOS: ARREGLO (ARRAY)
// ---------------------------------------------------------------------
// Se usa un arreglo de tamaño fijo (frames[MAX_FRAMES]) para almacenar los
// fotogramas (texturas) de una animación: el idle del personaje, cada uno
// de los 4 finales, el "video" de intro y el gif del menú (ambos se tratan
// como una secuencia de imágenes). Un arreglo es ideal porque la cantidad
// de fotogramas es conocida al cargar la animación y se necesita acceso
// directo por índice para saber "qué fotograma toca dibujar ahora".
// =====================================================================
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>

class ArrayAnimacion {
public:
    static const int MAX_FRAMES = 64; // tamaño máximo del arreglo de fotogramas

    ArrayAnimacion()
        : cantidadFrames(0), indiceActual(0), tiempoAcumulado(0.f),
          tiempoPorFrame(0.12f), enBucle(true), terminada(false) {}

    // Carga los fotogramas indicados dentro del arreglo interno.
    // segundosPorFrame: velocidad de la animación.
    // loop: true = se repite indefinidamente, false = se detiene en el último frame.
    bool cargar(const std::vector<std::string>& rutas, float segundosPorFrame, bool loop) {
        cantidadFrames = 0;
        tiempoPorFrame = segundosPorFrame;
        enBucle = loop;
        terminada = false;
        indiceActual = 0;
        tiempoAcumulado = 0.f;

        for (const auto& ruta : rutas) {
            if (cantidadFrames >= MAX_FRAMES) break;
            if (!frames[cantidadFrames].loadFromFile(ruta)) {
                std::cerr << "[ArrayAnimacion] No se pudo cargar: " << ruta << std::endl;
                return false;
            }
            cantidadFrames++;
        }
        return cantidadFrames > 0;
    }

    void reiniciar() {
        indiceActual = 0;
        tiempoAcumulado = 0.f;
        terminada = false;
    }

    // Avanza el índice del arreglo según el tiempo transcurrido (dt en segundos)
    void actualizar(float dt) {
        if (cantidadFrames <= 1 || terminada) return;
        tiempoAcumulado += dt;
        if (tiempoAcumulado >= tiempoPorFrame) {
            tiempoAcumulado -= tiempoPorFrame;
            if (indiceActual < cantidadFrames - 1) {
                indiceActual++;
            } else if (enBucle) {
                indiceActual = 0;
            } else {
                terminada = true;
            }
        }
    }

    bool haTerminado() const { return terminada; }
    int getCantidadFrames() const { return cantidadFrames; }
    int getIndiceActual() const { return indiceActual; }

    const sf::Texture& getTexturaActual() const {
        return frames[indiceActual];
    }

private:
    sf::Texture frames[MAX_FRAMES]; // <-- EL ARREGLO
    int cantidadFrames;
    int indiceActual;
    float tiempoAcumulado;
    float tiempoPorFrame;
    bool enBucle;
    bool terminada;
};
