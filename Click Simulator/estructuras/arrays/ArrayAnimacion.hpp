#pragma once
// =====================================================================
// ESTRUCTURA DE DATOS: ARREGLO (ARRAY)
// ---------------------------------------------------------------------
// Se usa un arreglo de tamaño fijo (frames[MAX_FRAMES] o rectsHoja[MAX_FRAMES])
// para almacenar los fotogramas de una animación: el idle del personaje,
// cada uno de los 4 finales (un PNG por fotograma) y el gif del menú (que
// ahora se recorta a partir de un único sprite sheet). Un arreglo es ideal
// porque la cantidad de fotogramas es conocida al cargar la animación y se
// necesita acceso directo por índice para saber "qué fotograma toca dibujar
// ahora".
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
          tiempoPorFrame(0.12f), enBucle(true), terminada(false), esHoja(false) {}

    // Carga los fotogramas indicados dentro del arreglo interno (un archivo
    // PNG por fotograma). segundosPorFrame: velocidad de la animación.
    // loop: true = se repite indefinidamente, false = se detiene en el último frame.
    bool cargar(const std::vector<std::string>& rutas, float segundosPorFrame, bool loop) {
        esHoja = false;
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

    // Carga los fotogramas a partir de UNA sola imagen "sprite sheet" (el gif
    // ya convertido en tira de imágenes, como el que arma la herramienta de
    // conversión). columnas/filas: cuadrícula de la hoja. cantidadFramesUsados:
    // cuántas celdas de esa cuadrícula son fotogramas reales (de izquierda a
    // derecha, de arriba hacia abajo); si es 0 o mayor a columnas*filas, se
    // usan todas las celdas.
    bool cargarSpriteSheet(const std::string& rutaImagen, int columnas, int filas,
                            int cantidadFramesUsados, float segundosPorFrame, bool loop) {
        if (!texturaHoja.loadFromFile(rutaImagen)) {
            std::cerr << "[ArrayAnimacion] No se pudo cargar el sprite sheet: " << rutaImagen << std::endl;
            return false;
        }

        esHoja = true;
        tiempoPorFrame = segundosPorFrame;
        enBucle = loop;
        terminada = false;
        indiceActual = 0;
        tiempoAcumulado = 0.f;
        cantidadFrames = 0;

        int col = columnas > 0 ? columnas : 1;
        int fil = filas > 0 ? filas : 1;
        sf::Vector2u tamHoja = texturaHoja.getSize();
        int anchoFrame = static_cast<int>(tamHoja.x) / col;
        int altoFrame = static_cast<int>(tamHoja.y) / fil;

        int totalCeldas = col * fil;
        int usar = (cantidadFramesUsados > 0 && cantidadFramesUsados < totalCeldas)
                       ? cantidadFramesUsados
                       : totalCeldas;

        for (int i = 0; i < usar && cantidadFrames < MAX_FRAMES; ++i) {
            int fila = i / col;
            int columna = i % col;
            rectsHoja[cantidadFrames] = sf::IntRect(columna * anchoFrame, fila * altoFrame,
                                                     anchoFrame, altoFrame);
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

    // Modo "un archivo por fotograma": la textura completa ES el fotograma.
    // Modo "sprite sheet": devuelve la hoja completa (hay que combinarla con
    // getRectActual() para recortar el fotograma correcto). Se recomienda
    // usar aplicarA(sprite) en vez de llamar esto directamente.
    const sf::Texture& getTexturaActual() const {
        return esHoja ? texturaHoja : frames[indiceActual];
    }

    // Recorte dentro de la hoja para el fotograma actual (solo aplica en
    // modo sprite sheet).
    sf::IntRect getRectActual() const {
        return rectsHoja[indiceActual];
    }

    bool esSpriteSheet() const { return esHoja; }

    // Configura un sf::Sprite con el fotograma actual, sea cual sea el modo
    // en el que se cargó esta animación (archivos sueltos o sprite sheet).
    void aplicarA(sf::Sprite& sprite) const {
        if (esHoja) {
            sprite.setTexture(texturaHoja, false);
            sprite.setTextureRect(rectsHoja[indiceActual]);
        } else {
            sprite.setTexture(frames[indiceActual], true);
        }
    }

private:
    sf::Texture frames[MAX_FRAMES]; // <-- EL ARREGLO (modo archivos sueltos)
    sf::Texture texturaHoja;        // <-- la imagen del sprite sheet (modo hoja)
    sf::IntRect rectsHoja[MAX_FRAMES]; // <-- EL ARREGLO de recortes dentro de la hoja
    int cantidadFrames;
    int indiceActual;
    float tiempoAcumulado;
    float tiempoPorFrame;
    bool enBucle;
    bool terminada;
    bool esHoja;
};
