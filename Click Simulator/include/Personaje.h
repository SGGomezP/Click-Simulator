#pragma once
#include <SFML/Graphics.hpp>
#include "../estructuras/arrays/ArrayAnimacion.hpp"
#include <string>
#include <vector>

enum class TipoFinal { NINGUNO, BESO, SALUDO, POSE, PATINETA };

// El personaje es el "botón" clickeable del juego. Internamente alterna
// entre su animación idle (arreglo de 5 frames en bucle) y, al llegar a un
// final, la animación de ese final (arreglo de 4/4/8/2 frames según el caso).
class Personaje {
public:
    Personaje();

    bool cargarIdle(const std::vector<std::string>& rutas);
    bool cargarFinalBeso(const std::vector<std::string>& rutas);
    bool cargarFinalSaludo(const std::vector<std::string>& rutas);
    bool cargarFinalPose(const std::vector<std::string>& rutas);
    bool cargarFinalPatineta(const std::vector<std::string>& rutas);

    void establecerPosicion(float x, float y);

    void actualizar(float dt, float anchoVentana, float altoVentana);
    void dibujar(sf::RenderWindow& ventana);

    bool contienePunto(sf::Vector2f punto) const;
    void alHacerClick(); // dispara el efecto de "achicarse"

    void iniciarFinal(TipoFinal tipo);
    bool finalSalioDePantalla() const { return salioDePantalla; }
    TipoFinal getFinalActivo() const { return finalActivo; }

private:
    ArrayAnimacion animIdle;
    ArrayAnimacion animBeso;
    ArrayAnimacion animSaludo;
    ArrayAnimacion animPose;
    ArrayAnimacion animPatineta;

    sf::Sprite sprite;
    sf::Vector2f posicionBase;

    // El personaje se escala automáticamente para que, sin importar el
    // tamaño real en píxeles del sprite que se cargó, siempre ocupe una
    // fracción fija del alto de la ventana (y así no tape todo el fondo).
    // ajusteFino permite compensar a mano si aun así se ve muy grande/chico.
    static constexpr float FRACCION_ALTO_VENTANA = 0.55f;
    float ajusteFino; // multiplicador extra sobre la escala automática (1.0 = sin cambios)

    // efecto de click (achicarse un poco y volver a la normalidad)
    float tiempoEfectoClick;
    static constexpr float DURACION_EFECTO_CLICK = 0.12f;

    TipoFinal finalActivo;
    bool deslizandoIzquierda;
    bool deslizandoDerecha;
    bool salioDePantalla;
    static constexpr float VELOCIDAD_DESLIZAMIENTO = 260.f; // px/seg
};
