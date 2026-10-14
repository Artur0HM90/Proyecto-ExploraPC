#pragma once
#include "Componentes.h"

enum class Modo { Libre, Guiado };

// Sólo guarda el estado educativo. No conoce OpenGL, ventanas ni botones.
class Aprendizaje {
public:
    Modo modo = Modo::Libre;
    int paso = 0;
    bool verDemostracion = false;
    bool iniciada = false, reproduciendo = false;
    float tiempo = 0;
    static constexpr float duracionFase = 4.f, duracion = 12.f;
    static constexpr int orden[5] = {4,0,3,2,1};

    void explorar();
    void guiar(Componentes& piezas);
    void avanzar(int direccion, Componentes& piezas);
    void abrirDemostracion(Componentes& piezas);
    void seleccionar(int indice, Componentes& piezas);
    void reproducir();
    void pausarContinuar();
    void reiniciar();
    void actualizar(float segundos, Componentes& piezas);
    int fase() const;
    int componenteFase() const;
    const char* tituloFase() const;
    const char* explicacionFase() const;
    std::array<glm::vec3,3> rutaDatos(const Componentes& piezas) const;
    glm::vec3 posicionDato(const Componentes& piezas, int simbolo) const;
};
