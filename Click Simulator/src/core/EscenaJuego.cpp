#include "../../include/EscenaJuego.h"
#include "../../include/Utilidades.h"
#include <iostream>
#include <cstdio>

static const std::string RUTA_GUARDADO = "guardado/progreso.dat";

EscenaJuego::EscenaJuego()
    : ancho(800.f), alto(600.f), fuenteCargada(false), audio(nullptr),
      contadorClicks(0), juegoTerminado(false),
      dialogoAbierto(false), nodoActivo(nullptr), acumuladorRevelado(0.f),
      mostrarTexto(false), mostrarOpcionIzq(false), mostrarOpcionDer(false),
      despedidaSonada(false) {}

void EscenaJuego::establecerAudio(ManejadorAudio* a) { audio = a; }

bool EscenaJuego::cargar(float anchoVentana, float altoVentana) {
    ancho = anchoVentana;
    alto = altoVentana;

    // --- fondo del gameplay (la imagen original del proyecto) ---
    if (!texturaFondo.loadFromFile("assets/images/fondo.png")) {
        std::cerr << "[EscenaJuego] No se pudo cargar assets/images/fondo.png" << std::endl;
    } else {
        spriteFondo.setTexture(texturaFondo, true);
        ajustarSpriteAVentana(spriteFondo, texturaFondo, ancho, alto);
        spriteFondo.setPosition(0.f, 0.f);
    }

    // --- personaje: idle (5 frames) ---
    auto rutasIdle = generarRutasFrames("assets/images/personaje/idle", "idle_", ".png");
    if (rutasIdle.empty() || !personaje.cargarIdle(rutasIdle)) {
        std::cerr << "[EscenaJuego] Faltan sprites idle en assets/images/personaje/idle/ "
                     "(idle_0001.png .. idle_0005.png)" << std::endl;
    }
    personaje.establecerPosicion(ancho * 0.5f, alto * 0.52f);

    // --- finales ---
    auto rutasBeso = generarRutasFrames("assets/images/personaje/final_beso", "beso_", ".png");
    if (!rutasBeso.empty()) personaje.cargarFinalBeso(rutasBeso);
    else std::cerr << "[EscenaJuego] Faltan sprites en final_beso/ (beso_0001..0004.png)" << std::endl;

    auto rutasSaludo = generarRutasFrames("assets/images/personaje/final_saludo", "saludo_", ".png");
    if (!rutasSaludo.empty()) personaje.cargarFinalSaludo(rutasSaludo);
    else std::cerr << "[EscenaJuego] Faltan sprites en final_saludo/ (saludo_0001..0004.png)" << std::endl;

    auto rutasPose = generarRutasFrames("assets/images/personaje/final_pose", "pose_", ".png");
    if (!rutasPose.empty()) personaje.cargarFinalPose(rutasPose);
    else std::cerr << "[EscenaJuego] Faltan sprites en final_pose/ (pose_0001..0008.png)" << std::endl;

    auto rutasPatineta = generarRutasFrames("assets/images/personaje/final_patineta", "patineta_", ".png");
    if (!rutasPatineta.empty()) personaje.cargarFinalPatineta(rutasPatineta);
    else std::cerr << "[EscenaJuego] Faltan sprites en final_patineta/ (patineta_0001..0002.png)" << std::endl;

    // --- flor + contador ---
    if (!texturaFlor.loadFromFile("assets/images/ui/flor.png")) {
        std::cerr << "[EscenaJuego] Falta assets/images/ui/flor.png" << std::endl;
    } else {
        spriteFlor.setTexture(texturaFlor, true);
        spriteFlor.setScale(2.f, 2.f);
        spriteFlor.setPosition(20.f, alto - 60.f);
    }

    // --- fuente ---
    fuenteCargada = fuente.loadFromFile("assets/fuentes/pixel_font.ttf");
    if (!fuenteCargada) {
        std::cerr << "[EscenaJuego] Falta assets/fuentes/pixel_font.ttf (los textos no se veran)" << std::endl;
    }

    // --- textos y cajas de UI ---
    textoContador.setCharacterSize(28);
    textoContador.setFillColor(sf::Color::White);
    textoContador.setOutlineColor(sf::Color::Black);
    textoContador.setOutlineThickness(2.f);
    textoContador.setPosition(60.f, alto - 55.f);
    if (fuenteCargada) textoContador.setFont(fuente);

    auto prepararCaja = [&](sf::RectangleShape& caja, sf::RectangleShape& borde, sf::Vector2f tam, sf::Vector2f pos) {
        borde.setSize(sf::Vector2f(tam.x + 8.f, tam.y + 8.f));
        borde.setPosition(pos.x - 4.f, pos.y - 4.f);
        borde.setFillColor(sf::Color::White);
        caja.setSize(tam);
        caja.setPosition(pos);
        caja.setFillColor(sf::Color::Black);
    };

    prepararCaja(cajaPregunta, bordePregunta, sf::Vector2f(560.f, 100.f), sf::Vector2f(ancho * 0.5f - 280.f, 90.f));
    prepararCaja(cajaOpcionIzq, bordeOpcionIzq, sf::Vector2f(230.f, 60.f), sf::Vector2f(ancho * 0.25f - 115.f, alto * 0.78f));
    prepararCaja(cajaOpcionDer, bordeOpcionDer, sf::Vector2f(230.f, 60.f), sf::Vector2f(ancho * 0.75f - 115.f, alto * 0.78f));

    auto prepararTexto = [&](sf::Text& texto, sf::RectangleShape& caja) {
        texto.setCharacterSize(22);
        texto.setFillColor(sf::Color::White);
        if (fuenteCargada) texto.setFont(fuente);
        texto.setPosition(caja.getPosition().x + 14.f, caja.getPosition().y + caja.getSize().y / 2.f - 14.f);
    };
    prepararTexto(textoPregunta, cajaPregunta);
    prepararTexto(textoOpcionIzq, cajaOpcionIzq);
    prepararTexto(textoOpcionDer, cajaOpcionDer);

    textoFin.setString("FIN. CIERRA EL JUEGO");
    textoFin.setCharacterSize(34);
    textoFin.setFillColor(sf::Color::Yellow);
    textoFin.setOutlineColor(sf::Color::Black);
    textoFin.setOutlineThickness(2.f);
    if (fuenteCargada) textoFin.setFont(fuente);
    sf::FloatRect b = textoFin.getLocalBounds();
    textoFin.setOrigin(b.width / 2.f, 0.f);
    textoFin.setPosition(ancho / 2.f, 20.f);

    construirGrafo();
    cargarProgreso();

    return true;
}

void EscenaJuego::construirGrafo() {
    // Árbol de diálogo (GRAFO):
    //
    //            N10 "Hola nena"
    //           /              \
    //   Coquetear              Ignorar
    //        |                     |
    //     N20_A                 N20_B
    //   "Da una flor"      "Como te llamas?"
    //    /        \             /        \
    // Aceptas   No gracias  Decir nombre  Queti
    //    |          |            |          |
    //   F1(beso)  F2(saludo)  F3(pose)   F4(patineta)
    grafo.agregarNodo("N10", "Hola nena");
    grafo.agregarArista("N10", "Coquetear", "N20_A");
    grafo.agregarArista("N10", "Ignorar", "N20_B");

    grafo.agregarNodo("N20_A", "Da una flor");
    grafo.agregarArista("N20_A", "Aceptas", "F1");
    grafo.agregarArista("N20_A", "No gracias", "F2");

    grafo.agregarNodo("N20_B", "Como te llamas?");
    grafo.agregarArista("N20_B", "Decir nombre", "F3");
    grafo.agregarArista("N20_B", "Queti", "F4");
}

void EscenaJuego::cargarProgreso() {
    if (!archivoExiste(RUTA_GUARDADO)) return;
    guardado.cargarDesdeArchivo(RUTA_GUARDADO);

    try { contadorClicks = std::stoi(guardado.get("clicks", "0")); }
    catch (...) { contadorClicks = 0; }

    nodoParaMostrar = guardado.get("nodoParaMostrar", "");
    finalPendienteId = guardado.get("finalPendiente", "");
    std::string nodoAbierto = guardado.get("nodoAbierto", "");

    if (!nodoAbierto.empty()) {
        abrirDialogo(nodoAbierto);
        // Se restaura ya revelado por completo, sin animación de aparición.
        mostrarTexto = mostrarOpcionIzq = mostrarOpcionDer = true;
        while (!colaDialogo.estaVacia()) colaDialogo.desencolar();
    }
}

void EscenaJuego::guardarProgreso() {
    guardado.set("clicks", std::to_string(contadorClicks));
    guardado.set("nodoParaMostrar", nodoParaMostrar);
    guardado.set("finalPendiente", finalPendienteId);
    guardado.set("nodoAbierto", (dialogoAbierto && nodoActivo) ? nodoActivo->id : "");
    guardado.guardarEnArchivo(RUTA_GUARDADO);
}

void EscenaJuego::borrarProgreso() {
    std::remove(RUTA_GUARDADO.c_str());
}

void EscenaJuego::alCerrarVentana() {
    if (juegoTerminado) borrarProgreso();
    else guardarProgreso();
}

void EscenaJuego::abrirDialogo(const std::string& idNodo) {
    const NodoInteraccion* nodo = grafo.obtenerNodo(idNodo);
    if (!nodo) return;

    nodoActivo = nodo;
    dialogoAbierto = true;
    acumuladorRevelado = 0.f;
    mostrarTexto = mostrarOpcionIzq = mostrarOpcionDer = false;

    colaDialogo.vaciar();
    colaDialogo.encolar(EventoDialogo{"TEXTO", nodo->texto});
    if (nodo->opciones.size() > 0) colaDialogo.encolar(EventoDialogo{"OPCION_IZQUIERDA", nodo->opciones[0].etiqueta});
    if (nodo->opciones.size() > 1) colaDialogo.encolar(EventoDialogo{"OPCION_DERECHA", nodo->opciones[1].etiqueta});

    textoPregunta.setString(nodo->texto);
    if (nodo->opciones.size() > 0) textoOpcionIzq.setString(nodo->opciones[0].etiqueta);
    if (nodo->opciones.size() > 1) textoOpcionDer.setString(nodo->opciones[1].etiqueta);
}

void EscenaJuego::elegirOpcion(const OpcionInteraccion& opcion) {
    if (grafo.esNodoDeDialogo(opcion.destinoId)) {
        nodoParaMostrar = opcion.destinoId; // se abrirá en el siguiente umbral (20 clicks)
    } else {
        finalPendienteId = opcion.destinoId; // "F1".."F4", se activará en el click 30
    }

    dialogoAbierto = false;
    nodoActivo = nullptr;
    colaDialogo.vaciar();
    mostrarTexto = mostrarOpcionIzq = mostrarOpcionDer = false;

    guardarProgreso();
}

TipoFinal EscenaJuego::finalDesdeId(const std::string& idFinal) const {
    if (idFinal == "F1") return TipoFinal::BESO;
    if (idFinal == "F2") return TipoFinal::SALUDO;
    if (idFinal == "F3") return TipoFinal::POSE;
    if (idFinal == "F4") return TipoFinal::PATINETA;
    return TipoFinal::NINGUNO;
}

void EscenaJuego::activarFinalSiCorresponde() {
    juegoTerminado = true;
    TipoFinal tipo = finalDesdeId(finalPendienteId);
    personaje.iniciarFinal(tipo);
    if (audio && !despedidaSonada) {
        audio->reproducirDespedida();
        despedidaSonada = true;
    }
}

void EscenaJuego::manejarClickPersonaje() {
    contadorClicks++;
    personaje.alHacerClick();
    if (audio) audio->reproducirVozClickAleatoria();

    if (contadorClicks == 10) {
        abrirDialogo("N10");
    } else if (contadorClicks == 20) {
        if (!nodoParaMostrar.empty()) abrirDialogo(nodoParaMostrar);
    } else if (contadorClicks >= 30) {
        activarFinalSiCorresponde();
    }

    guardarProgreso();
}

void EscenaJuego::procesarEventos(const sf::Event& evento) {
    if (evento.type != sf::Event::MouseButtonPressed || evento.mouseButton.button != sf::Mouse::Left) return;

    sf::Vector2f click(static_cast<float>(evento.mouseButton.x), static_cast<float>(evento.mouseButton.y));

    if (dialogoAbierto && nodoActivo) {
        if (mostrarOpcionIzq && nodoActivo->opciones.size() > 0 && cajaOpcionIzq.getGlobalBounds().contains(click)) {
            elegirOpcion(nodoActivo->opciones[0]);
        } else if (mostrarOpcionDer && nodoActivo->opciones.size() > 1 && cajaOpcionDer.getGlobalBounds().contains(click)) {
            elegirOpcion(nodoActivo->opciones[1]);
        }
        return; // mientras el dialogo esta abierto no se puede clickear al personaje
    }

    if (!juegoTerminado && personaje.contienePunto(click)) {
        manejarClickPersonaje();
    }
}

void EscenaJuego::actualizar(float dt) {
    personaje.actualizar(dt, ancho);
    textoContador.setString(std::to_string(contadorClicks));

    if (dialogoAbierto) {
        acumuladorRevelado += dt;
        if (acumuladorRevelado >= 0.25f && !colaDialogo.estaVacia()) {
            acumuladorRevelado = 0.f;
            EventoDialogo ev = colaDialogo.desencolar(); // COLA: revela en orden FIFO
            if (ev.tipo == "TEXTO") mostrarTexto = true;
            else if (ev.tipo == "OPCION_IZQUIERDA") mostrarOpcionIzq = true;
            else if (ev.tipo == "OPCION_DERECHA") mostrarOpcionDer = true;
        }
    }
}

void EscenaJuego::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(spriteFondo);
    personaje.dibujar(ventana);
    ventana.draw(spriteFlor);
    ventana.draw(textoContador);

    if (dialogoAbierto) {
        if (mostrarTexto) {
            ventana.draw(bordePregunta);
            ventana.draw(cajaPregunta);
            ventana.draw(textoPregunta);
        }
        if (mostrarOpcionIzq) {
            ventana.draw(bordeOpcionIzq);
            ventana.draw(cajaOpcionIzq);
            ventana.draw(textoOpcionIzq);
        }
        if (mostrarOpcionDer) {
            ventana.draw(bordeOpcionDer);
            ventana.draw(cajaOpcionDer);
            ventana.draw(textoOpcionDer);
        }
    }

    if (juegoTerminado) {
        ventana.draw(textoFin);
    }
}
