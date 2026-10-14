#include "Camara.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

void Camara::restablecer() { azimut_ = .32f; elevacion_ = .16f; distancia_ = 10.8f; }
void Camara::girar(float dx, float dy) {
    azimut_ = std::remainder(azimut_ - dx*.006f, 6.2831853f);
    // Evitamos pasar por los polos, donde la orientación de lookAt es ambigua.
    elevacion_ = std::clamp(elevacion_ + dy*.006f, -1.1f, 1.1f);
}
void Camara::acercar(float pasos) { distancia_ = std::clamp(distancia_ - pasos*.65f, 6.5f, 20.f); }
bool Camara::entrada(float x,float y,bool pulsado,bool permitido,float rueda) {
    bool clic=false;
    if(pulsado && !anteriorPulsado_) {
        gestoEscena_=permitido; arrastrando_=false; inicioX_=x; inicioY_=y;
    }
    if(anteriorPulsado_ && gestoEscena_) {
        float dx=x-inicioX_,dy=y-inicioY_;
        bool superaUmbral=dx*dx+dy*dy>25.f; // Cinco puntos de pantalla.
        if(pulsado && permitido) {
            if(arrastrando_) girar(x-previoX_,y-previoY_);
            else if(superaUmbral) girar(dx,dy);
        }
        arrastrando_=arrastrando_ || superaUmbral;
        if(!pulsado) clic=permitido && !arrastrando_;
    }
    if(!pulsado) gestoEscena_=false;
    anteriorPulsado_=pulsado; previoX_=x; previoY_=y;
    if(permitido) acercar(rueda);
    return clic;
}
glm::mat4 Camara::vistaProyeccion(float aspecto) const {
    glm::vec3 objetivo(0,0,.35f);
    glm::vec3 ojo = objetivo + distancia_ * glm::vec3(
        std::cos(elevacion_)*std::sin(azimut_), std::sin(elevacion_),
        std::cos(elevacion_)*std::cos(azimut_));
    return glm::perspective(glm::radians(42.f),aspecto,.1f,80.f)
         * glm::lookAt(ojo,objetivo,glm::vec3(0,1,0));
}
