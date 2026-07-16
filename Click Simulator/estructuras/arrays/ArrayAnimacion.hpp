#pragma once
// =====================================================================
// ESTRUCTURA DE DATOS: ARREGLO ESTÁTICO (ARRAY)
// ---------------------------------------------------------------------
// Se usa un arreglo de tamaño fijo (frames[MAX_FRAMES] o rectsHoja[MAX_FRAMES])
// para almacenar los fotogramas de una animación de forma contigua en memoria.
//
// Al NO usar la STL (sin std::vector ni std::string), garantizamos:
// 1. Cero fragmentación de memoria en tiempo de ejecución.
// 2. Acceso directo por índice en tiempo constante O(1).
// =====================================================================
#include <SFML/Graphics.hpp>
#include <iostream> // Para depuración básica en consola (std::cerr)

class ArrayAnimacion {
public:
    static const int MAX_FRAMES = 64; // Límite físico para evitar desbordamientos

    ArrayAnimacion()
        : cantidadFrames(0), indiceActual(0), tiempoAcumulado(0.f),
          tiempoPorFrame(0.12f), enBucle(true), terminada(false), esHoja(false) {}

    // Carga los fotogramas usando un arreglo de rutas de estilo C (const char*)
    // Reemplaza std::vector<std::string>
    bool cargar(const char* rutas[], int cantidadRutas, float segundosPorFrame, bool loop) {
        esHoja = false;
        cantidadFrames = 0;
        tiempoPorFrame = segundosPorFrame;
        enBucle = loop;
        terminada = false;
        indiceActual = 0;
        tiempoAcumulado = 0.f;

        for (int i = 0; i < cantidadRutas; ++i) {
            // Caso límite (Overflow): No permitir almacenar más allá del límite estático
            if (cantidadFrames >= MAX_FRAMES) break;

            // SFML acepta implícitamente const char* para cargar archivos
            if (!frames[cantidadFrames].loadFromFile(rutas[i])) {
                std::cerr << "[ArrayAnimacion] No se pudo cargar: " << rutas[i] << std::endl;
                return false;
            }
            cantidadFrames++;
        }
        return cantidadFrames > 0;
    }

    // Carga usando una ruta de estilo C (const char*)
    // Reemplaza std::string
    bool cargarSpriteSheet(const char* rutaImagen, int columnas, int filas,
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

        // Mapeo bidimensional a un arreglo unidimensional de recortes
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

    // Avanza el índice del arreglo según el tiempo transcurrido (dt)
    void actualizar(float dt) {
        if (cantidadFrames <= 1 || terminada) return;

        tiempoAcumulado += dt;
        if (tiempoAcumulado >= tiempoPorFrame) {
            tiempoAcumulado -= tiempoPorFrame;
            
            // Lógica de avance y control de límites
            if (indiceActual < cantidadFrames - 1) {
                indiceActual++;
            } else if (enBucle) {
                indiceActual = 0; // Reinicio en ciclo
            } else {
                terminada = true; // Fin de animación sin bucle
            }
        }
    }

    bool haTerminado() const { return terminada; }
    int getCantidadFrames() const { return cantidadFrames; }
    int getIndiceActual() const { return indiceActual; }

    const sf::Texture& getTexturaActual() const {
        return esHoja ? texturaHoja : frames[indiceActual];
    }

    sf::IntRect getRectActual() const {
        return rectsHoja[indiceActual];
    }

    bool esSpriteSheet() const { return esHoja; }

    void aplicarA(sf::Sprite& sprite) const {
        if (esHoja) {
            sprite.setTexture(texturaHoja, false);
            sprite.setTextureRect(rectsHoja[indiceActual]);
        } else {
            sprite.setTexture(frames[indiceActual], true);
        }
    }

private:
    sf::Texture frames[MAX_FRAMES];        // Arreglo estático de texturas
    sf::Texture texturaHoja;               // Textura única para hojas de sprites
    sf::IntRect rectsHoja[MAX_FRAMES];     // Arreglo estático de coordenadas de recorte
    int cantidadFrames;
    int indiceActual;
    float tiempoAcumulado;
    float tiempoPorFrame;
    bool enBucle;
    bool terminada;
    bool esHoja;
};