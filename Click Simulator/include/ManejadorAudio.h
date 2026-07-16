#pragma once
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <memory>

// Maneja la música en bucle del menú y del juego, las voces de click
// aleatorias (con cooldown) y el sonido de despedida final.
class ManejadorAudio {
public:
    ManejadorAudio();

    bool cargarMusicaMenu(const std::string& ruta);
    bool cargarMusicaJuego(const std::string& ruta);
    bool cargarVocesClick(const std::vector<std::string>& rutas); // hasta 10 clips
    bool cargarSonidoDespedida(const std::string& ruta);

    void reproducirMusicaMenu();
    void reproducirMusicaJuego();
    void detenerMusica();

    // Intenta reproducir un clip de voz aleatorio. Si no ha pasado el
    // cooldown (0.5s) desde el último clip, no hace nada y devuelve false.
    bool reproducirVozClickAleatoria();

    void reproducirDespedida();

private:
    sf::Music musicaMenu;
    sf::Music musicaJuego;
    bool musicaMenuCargada;
    bool musicaJuegoCargada;

    std::vector<std::shared_ptr<sf::SoundBuffer>> buffersVoces;
    sf::Sound sonidoVoz;

    sf::SoundBuffer bufferDespedida;
    sf::Sound sonidoDespedida;
    bool despedidaCargada;

    sf::Clock relojCooldown;
    float cooldownSegundos;
};
