#pragma once
// =====================================================================
// ESTRUCTURA DE DATOS: TABLA HASH (HASH TABLE / unordered_map)
// ---------------------------------------------------------------------
// Se usa una tabla hash (clave -> valor) para GUARDAR EL PROGRESO de la
// partida: número de clicks, nodo de diálogo actual, final pendiente,
// etc. Cada dato se guarda y se recupera por su clave (ej: "clicks",
// "nodoActual") en tiempo O(1) promedio, sin importar cuántos datos haya
// ni el orden en que se guardaron, que es justo lo que se necesita para
// un archivo de guardado con varios campos sueltos.
//
// Es la respuesta a "tablas hash pero no sé para qué": aquí se usa como
// el "expediente" de la partida (clicks, elección 1, elección 2, final)
// que se lee/escribe en un archivo de texto plano "clave=valor".
// =====================================================================
#include <unordered_map>
#include <string>
#include <fstream>

class TablaHashGuardado {
public:
    void set(const std::string& clave, const std::string& valor) {
        datos[clave] = valor;
    }

    std::string get(const std::string& clave, const std::string& porDefecto = "") const {
        auto it = datos.find(clave);
        if (it == datos.end()) return porDefecto;
        return it->second;
    }

    bool existe(const std::string& clave) const {
        return datos.find(clave) != datos.end();
    }

    void limpiar() { datos.clear(); }

    // Guarda la tabla completa en un archivo de texto plano "clave=valor"
    bool guardarEnArchivo(const std::string& ruta) const {
        std::ofstream archivo(ruta);
        if (!archivo.is_open()) return false;
        for (const auto& par : datos) {
            archivo << par.first << "=" << par.second << "\n";
        }
        return true;
    }

    // Carga la tabla desde un archivo de texto plano "clave=valor"
    bool cargarDesdeArchivo(const std::string& ruta) {
        std::ifstream archivo(ruta);
        if (!archivo.is_open()) return false;
        datos.clear();
        std::string linea;
        while (std::getline(archivo, linea)) {
            auto pos = linea.find('=');
            if (pos == std::string::npos) continue;
            std::string clave = linea.substr(0, pos);
            std::string valor = linea.substr(pos + 1);
            datos[clave] = valor;
        }
        return true;
    }

private:
    std::unordered_map<std::string, std::string> datos; // la tabla hash real
};
