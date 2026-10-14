#include "Aprendizaje.h"
#include <algorithm>
#include <cmath>

void Aprendizaje::reiniciar() { tiempo=0; iniciada=false; reproduciendo=false; }
void Aprendizaje::explorar() {
    modo=Modo::Libre; verDemostracion=false; reiniciar();
}
void Aprendizaje::guiar(Componentes& piezas) {
    explorar(); modo=Modo::Guiado; paso=0; piezas.seleccionar(orden[paso]);
}
void Aprendizaje::avanzar(int direccion, Componentes& piezas) {
    if(modo!=Modo::Guiado) return;
    paso=std::clamp(paso+direccion,0,4); piezas.seleccionar(orden[paso]);
}
void Aprendizaje::abrirDemostracion(Componentes& piezas) {
    explorar(); verDemostracion=true; piezas.seleccionar(componenteFase());
}
void Aprendizaje::seleccionar(int indice, Componentes& piezas) {
    // En el recorrido, sólo Anterior/Siguiente cambian el paso. Un clic libre
    // abandona la demostración para mostrar la explicación de esa pieza.
    if(modo!=Modo::Libre || indice<0 || indice>=5) return;
    explorar(); piezas.seleccionar(indice);
}
void Aprendizaje::reproducir() {
    if(!verDemostracion) return;
    tiempo=0; iniciada=true; reproduciendo=true;
}
void Aprendizaje::pausarContinuar() {
    if(verDemostracion && iniciada && tiempo<duracion) reproduciendo=!reproduciendo;
}
int Aprendizaje::fase() const { return std::min(2,int(tiempo/duracionFase)); }
int Aprendizaje::componenteFase() const { const int indices[]={3,2,1}; return indices[fase()]; }
void Aprendizaje::actualizar(float segundos, Componentes& piezas) {
    if(verDemostracion && reproduciendo) {
        tiempo=std::clamp(tiempo+std::max(0.f,segundos),0.f,duracion);
        if(tiempo>=duracion) reproduciendo=false;
    }
    // Una sola selección gobierna el nombre, el marcador y el resaltado 3D.
    if(modo==Modo::Guiado) piezas.seleccionar(orden[paso]);
    if(verDemostracion) piezas.seleccionar(componenteFase());
}
const char* Aprendizaje::tituloFase() const {
    const char* titulos[]={"1. El SSD conserva el programa","2. Se carga lo necesario en la RAM","3. El procesador ejecuta tareas"};
    return titulos[fase()];
}
const char* Aprendizaje::explicacionFase() const {
    const char* textos[]={
        "El SSD conserva el programa y sus archivos, incluso cuando la computadora está apagada.",
        "Al abrir el programa, se cargan en la RAM las instrucciones y los datos necesarios desde el SSD. La placa madre permite la comunicación.",
        "El procesador utiliza la información de la RAM para ejecutar tareas. Puede leer y escribir datos en la RAM mientras trabaja."
    };
    return textos[fase()];
}
std::array<glm::vec3,3> Aprendizaje::rutaDatos(const Componentes& piezas) const {
    glm::vec3 placa=piezas.posicion(0)+glm::vec3(.15f,.15f,.30f);
    if(fase()<2) return {piezas.marcador(3),placa,piezas.marcador(2)};
    bool escritura=int((tiempo-8.f)/1.3f)%2!=0;
    int origen=escritura ? 1 : 2, destino=escritura ? 2 : 1;
    return {piezas.marcador(origen),placa,piezas.marcador(destino)};
}
glm::vec3 Aprendizaje::posicionDato(const Componentes& piezas,int simbolo) const {
    if(fase()==0) return piezas.marcador(3)+glm::vec3(float(simbolo-1)*.16f,.25f,0);
    auto ruta=rutaDatos(piezas);
    float ciclo=tiempo/1.8f+float(simbolo)/3.f;
    float t=(ciclo-std::floor(ciclo))*2.f;
    return t<1 ? glm::mix(ruta[0],ruta[1],t) : glm::mix(ruta[1],ruta[2],t-1.f);
}
