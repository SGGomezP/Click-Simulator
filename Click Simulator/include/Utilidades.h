#pragma once
// Funciones de apoyo usadas por varias escenas.
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>

inline bool archivoExiste(const std::string& ruta) {
    std::ifstream f(ruta.c_str());
    return f.good();
}

// Genera rutas del tipo "carpeta/prefijoNNNN.png" mientras el archivo exista,
// empezando en 0001. Así, para agregar/quitar fotogramas de una animación
// (video de intro, gif del menú, etc.) solo hay que agregar/quitar archivos,
// sin tocar el código.
inline std::vector<std::string> generarRutasFrames(const std::string& carpeta,
                                                     const std::string& prefijo,
                                                     const std::string& extension,
                                                     int maxFrames = 500) {
    std::vector<std::string> rutas;
    for (int i = 1; i <= maxFrames; ++i) {
        std::ostringstream nombre;
        nombre << carpeta << "/" << prefijo << std::setw(4) << std::setfill('0') << i << extension;
        std::string ruta = nombre.str();
        if (!archivoExiste(ruta)) break;
        rutas.push_back(ruta);
    }
    return rutas;
}

// Escala un sprite para que cubra exactamente el tamaño de ventana indicado,
// igual que ya se hacía con el fondo original del juego.
inline void ajustarSpriteAVentana(sf::Sprite& sprite, const sf::Texture& textura, float ancho, float alto) {
    sf::Vector2u tam = textura.getSize();
    if (tam.x == 0 || tam.y == 0) return;
    sprite.setScale(ancho / static_cast<float>(tam.x), alto / static_cast<float>(tam.y));
}
