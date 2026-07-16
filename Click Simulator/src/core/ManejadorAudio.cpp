#include "../../include/ManejadorAudio.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

ManejadorAudio::ManejadorAudio()
    : musicaMenuCargada(false), musicaJuegoCargada(false),
      despedidaCargada(false), cooldownSegundos(0.5f) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

bool ManejadorAudio::cargarMusicaMenu(const std::string& ruta) {
    musicaMenuCargada = musicaMenu.openFromFile(ruta);
    if (musicaMenuCargada) musicaMenu.setLoop(true);
    else std::cerr << "[Audio] No se pudo cargar musica de menu: " << ruta << std::endl;
    return musicaMenuCargada;
}

bool ManejadorAudio::cargarMusicaJuego(const std::string& ruta) {
    musicaJuegoCargada = musicaJuego.openFromFile(ruta);
    if (musicaJuegoCargada) musicaJuego.setLoop(true);
    else std::cerr << "[Audio] No se pudo cargar musica de juego: " << ruta << std::endl;
    return musicaJuegoCargada;
}

bool ManejadorAudio::cargarVocesClick(const std::vector<std::string>& rutas) {
    buffersVoces.clear();
    for (const auto& ruta : rutas) {
        auto buffer = std::make_shared<sf::SoundBuffer>();
        if (buffer->loadFromFile(ruta)) {
            buffersVoces.push_back(buffer);
        } else {
            std::cerr << "[Audio] No se pudo cargar voz: " << ruta << std::endl;
        }
    }
    return !buffersVoces.empty();
}

bool ManejadorAudio::cargarSonidoDespedida(const std::string& ruta) {
    despedidaCargada = bufferDespedida.loadFromFile(ruta);
    if (despedidaCargada) sonidoDespedida.setBuffer(bufferDespedida);
    else std::cerr << "[Audio] No se pudo cargar sonido de despedida: " << ruta << std::endl;
    return despedidaCargada;
}

void ManejadorAudio::reproducirMusicaMenu() {
    musicaJuego.stop();
    if (musicaMenuCargada) musicaMenu.play();
}

void ManejadorAudio::reproducirMusicaJuego() {
    musicaMenu.stop();
    if (musicaJuegoCargada) musicaJuego.play();
}

void ManejadorAudio::detenerMusica() {
    musicaMenu.stop();
    musicaJuego.stop();
}

bool ManejadorAudio::reproducirVozClickAleatoria() {
    if (buffersVoces.empty()) return false;
    if (relojCooldown.getElapsedTime().asSeconds() < cooldownSegundos) return false;

    int indice = std::rand() % static_cast<int>(buffersVoces.size());
    sonidoVoz.setBuffer(*buffersVoces[indice]);
    sonidoVoz.play();
    relojCooldown.restart();
    return true;
}

void ManejadorAudio::reproducirDespedida() {
    if (despedidaCargada) sonidoDespedida.play();
}
