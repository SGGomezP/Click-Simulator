#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Personaje.h"
#include "ManejadorAudio.h"
#include "../estructuras/grafos/GrafoInteracciones.hpp"
#include "../estructuras/colas/ColaDialogos.hpp"
#include "../estructuras/tablas_hash/TablaHashGuardado.hpp"

// Escena de juego: el personaje clickeable, el contador de clicks, el
// árbol de diálogos (grafo) que se dispara a los 10/20 clicks, y los
// 4 finales posibles que se disparan a los 30 clicks.
class EscenaJuego {
public:
    EscenaJuego();

    bool cargar(float anchoVentana, float altoVentana);
    void establecerAudio(ManejadorAudio* audio);

    void procesarEventos(const sf::Event& evento);
    void actualizar(float dt);
    void dibujar(sf::RenderWindow& ventana);

    // Se llama cuando el usuario cierra la ventana estando en esta escena:
    // guarda el progreso, o lo borra si ya se llegó a un final.
    void alCerrarVentana();

private:
    void construirGrafo();
    void cargarProgreso();
    void guardarProgreso();
    void borrarProgreso();

    void manejarClickPersonaje();
    void abrirDialogo(const std::string& idNodo);
    void elegirOpcion(const OpcionInteraccion& opcion);
    void activarFinalSiCorresponde();
    TipoFinal finalDesdeId(const std::string& idFinal) const;

    float ancho, alto;

    sf::Texture texturaFondo;
    sf::Sprite spriteFondo;

    Personaje personaje;

    sf::Texture texturaFlor;
    sf::Sprite spriteFlor;

    sf::Font fuente;
    bool fuenteCargada;
    sf::Text textoContador;

    ManejadorAudio* audio;

    // --- progreso / estructuras de datos ---
    GrafoInteracciones grafo;      // GRAFO: árbol de preguntas y respuestas
    ColaDialogos colaDialogo;      // COLA: orden de aparición de los elementos del diálogo
    TablaHashGuardado guardado;    // TABLA HASH: persistencia del progreso

    int contadorClicks;
    std::string nodoParaMostrar;   // id del próximo nodo de diálogo a abrir (a los 10/20 clicks)
    std::string finalPendienteId;  // "F1".."F4", elegido en el diálogo de los 20 clicks
    bool juegoTerminado;

    // --- estado del cuadro de diálogo activo ---
    bool dialogoAbierto;
    const NodoInteraccion* nodoActivo;
    float acumuladorRevelado;
    bool mostrarTexto, mostrarOpcionIzq, mostrarOpcionDer;

    sf::RectangleShape cajaPregunta, bordePregunta;
    sf::Text textoPregunta;
    sf::RectangleShape cajaOpcionIzq, bordeOpcionIzq;
    sf::Text textoOpcionIzq;
    sf::RectangleShape cajaOpcionDer, bordeOpcionDer;
    sf::Text textoOpcionDer;

    sf::Text textoFin; // "FIN, CIERRA EL JUEGO" en amarillo
    bool despedidaSonada;
};
