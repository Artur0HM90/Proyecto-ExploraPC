#pragma once
#include <array>
#include <glm/glm.hpp>
class Dibujo;

struct Componente {
    const char* nombre;
    const char* funcion;
    const char* comparacion;
    const char* relacion;
    glm::vec3 posicion, desplazamiento, color;
};
class Componentes {
public:
    Componentes();
    std::array<Componente,5> piezas;
    int seleccionado = 0;
    bool separado = false;
    float avance = 0;
    void seleccionar(int indice);
    void actualizar(float segundos);
    glm::vec3 posicion(int indice) const;
    glm::vec3 marcador(int indice) const;
    void dibujar(Dibujo& dibujo, bool demostracion = false) const;
};
