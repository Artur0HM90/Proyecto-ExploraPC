#include "Componentes.h"
#include "Dibujo.h"
#include <algorithm>
#include <cmath>

Componentes::Componentes() : piezas{{
    {"Placa madre", "Conecta los componentes y permite que intercambien datos mediante sus circuitos y conectores.",
     "Como las calles de una ciudad: conectan distintos lugares para que puedan comunicarse.",
     "En ella se instalan el procesador y la RAM. Conecta el SSD y distribuye energía que recibe de la fuente.",
     {0,.15f,-.52f},{-1.25f,.15f,-.25f},{.16f,.67f,.43f}},
    {"Procesador (CPU)", "Ejecuta instrucciones de los programas y realiza cálculos y decisiones.",
     "Como una persona que sigue una receta: interpreta los pasos y realiza cada tarea.",
     "Lee y escribe datos en la RAM. Accede al SSD a través del sistema y se comunica mediante la placa madre.",
     {-.55f,.65f,-.30f},{-1.65f,1.10f,1.65f},{.96f,.62f,.22f}},
    {"Memoria RAM", "Guarda temporalmente los datos y programas que se están utilizando. Su contenido se pierde al apagar la computadora.",
     "Como una mesa de trabajo: allí colocas lo que necesitas para la tarea del momento.",
     "El procesador trabaja con datos de la RAM. Al abrir un programa, sus datos se cargan desde el SSD a la RAM.",
     {.75f,.50f,-.25f},{1.35f,1.10f,1.65f},{.36f,.67f,1.0f}},
    {"Unidad SSD", "Conserva archivos, programas y el sistema operativo incluso cuando la computadora está apagada.",
     "Como un archivador: guarda documentos para recuperarlos más adelante.",
     "Se conecta a la placa madre. Los programas y datos que se usan se cargan en la RAM para que el procesador trabaje con ellos.",
     {.80f,-1.25f,.20f},{1.50f,.70f,1.30f},{.72f,.42f,.95f}},
    {"Fuente de alimentación", "Convierte la corriente eléctrica de la toma en los voltajes que necesitan los componentes.",
     "Como una estación que adapta y reparte energía a los equipos de un edificio.",
     "Alimenta la placa madre y otros dispositivos mediante cables. Sin energía, el procesador, la RAM y el SSD no funcionan.",
     {-.65f,-1.82f,-.05f},{-1.05f,.05f,1.50f},{.96f,.36f,.40f}}
}} {}
void Componentes::seleccionar(int indice) { if (indice >= 0 && indice < 5) seleccionado = indice; }
void Componentes::actualizar(float segundos) {
    // Velocidad expresada por segundo: no depende de cuántos cuadros se dibujan.
    constexpr float duracion=1.25f;
    float paso=std::max(0.f,segundos)/duracion;
    avance=std::clamp(avance+(separado ? paso : -paso),0.f,1.f);
}
glm::vec3 Componentes::posicion(int indice) const {
    // smoothstep tiene velocidad cero al comenzar y al terminar.
    float suavizado = avance*avance*(3.f-2.f*avance);
    return piezas[indice].posicion + piezas[indice].desplazamiento * suavizado;
}
glm::vec3 Componentes::marcador(int indice) const {
    const glm::vec3 offsets[] = {{-1.10f,1.30f,.13f},{0,0,.22f},{.10f,.65f,.15f},{0,0,.20f},{-.35f,.20f,.68f}};
    return posicion(indice)+offsets[indice];
}
void Componentes::dibujar(Dibujo& d, bool demostracion) const {
    for (int i=0; i<5; ++i) {
        auto p = posicion(i); auto color = piezas[i].color;
        bool activo = i == seleccionado || (demostracion && (i==4 || i==0));
        auto caja = [&](glm::vec3 offset, glm::vec3 tamano, glm::vec3 tono, int textura=0) {
            d.caja(p+offset,tamano,tono,activo,textura);
        };
        const glm::vec3 oscuro(.10f,.14f,.20f), dorado(.88f,.72f,.30f);
        if (i == 0) {
            caja({0,0,0},{2.70f,3.10f,.12f},color,1);
            // Conectores, ranuras y pequeños chips facilitan reconocer la placa.
            caja({-1.15f,.20f,.11f},{.20f,1.10f,.15f},{.58f,.65f,.72f});
            for (float y : {-.75f,-1.05f}) caja({-.30f,y,.10f},{1.45f,.12f,.10f},oscuro);
            for (float x : {-.20f,.20f}) caja({x,-.25f,.12f},{.28f,.28f,.14f},oscuro);
            caja({-.55f,.50f,.10f},{.88f,.94f,.07f},oscuro);
        } else if (i == 1) {
            caja({0,0,0},{.72f,.72f,.20f},color);
            caja({0,0,.12f},{.56f,.56f,.05f},{.86f,.89f,.93f});
            for (float x : {-.39f,.39f}) caja({x,0,0},{.07f,.60f,.06f},dorado);
        } else if (i == 2) {
            for (float x : {-.16f,.16f}) {
                caja({x,0,0},{.20f,1.72f,.12f},color);
                for (float y : {-.55f,-.18f,.19f,.56f}) caja({x,y,.09f},{.14f,.24f,.06f},oscuro);
                caja({x,-.82f,0},{.20f,.12f,.14f},dorado);
            }
        } else if (i == 3) {
            caja({0,0,0},{1.05f,.58f,.30f},color);
            caja({-.10f,0,.16f},{.65f,.34f,.02f},{.87f,.88f,.95f});
            caja({.44f,-.23f,0},{.17f,.08f,.23f},dorado);
        } else {
            caja({0,0,0},{1.35f,.85f,1.18f},color,2);
            caja({0,0,.61f},{.80f,.60f,.04f},oscuro,2);
            // Líneas geométricas representan cables, sin simular circuitos eléctricos.
            for (int cable=0; cable<3; ++cable)
                caja({.69f+float(cable)*.08f,.22f,.20f},{.055f,.68f,.055f},dorado);
        }
    }
}
