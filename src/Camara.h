#pragma once
#include <glm/glm.hpp>

class Camara {
public:
    void restablecer();
    void girar(float dx, float dy);
    void acercar(float pasos);
    bool entrada(float x,float y,bool pulsado,bool permitido,float rueda);
    glm::mat4 vistaProyeccion(float aspecto) const;
    float distancia() const { return distancia_; }
    float azimut() const { return azimut_; }
    float elevacion() const { return elevacion_; }
private:
    float azimut_ = .32f, elevacion_ = .16f, distancia_ = 10.8f;
    float previoX_=0,previoY_=0;
    float inicioX_=0,inicioY_=0;
    bool anteriorPulsado_=false,gestoEscena_=false,arrastrando_=false;
};
