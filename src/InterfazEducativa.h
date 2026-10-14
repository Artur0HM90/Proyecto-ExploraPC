#pragma once
#include "Camara.h"
#include "Aprendizaje.h"
#include <imgui.h>

// Centros de controles y marcadores para comprobar la interacción sin coordenadas fijas.
struct ControlesUI {
    std::array<ImVec2,5> piezas{};
    ImVec2 separar{}, ensamblar{}, restablecer{};
    ImVec2 libre{}, guiado{}, abrirDemo{}, anterior{}, siguiente{}, volver{};
    ImVec2 reproducir{}, pausa{}, reiniciar{};
};
class InterfazEducativa {
public:
    ControlesUI controles;
    std::array<ImVec2,5> centrosMarcadores{};
    std::array<bool,5> marcadoresVisibles{};
    static void estilo();
    static float anchoPanel(float anchoVentana);
    void dibujar(Componentes& componentes, Camara& camara, Aprendizaje& aprendizaje,
                 float ancho, float alto);
    void marcadores(const Componentes& componentes, const Aprendizaje& aprendizaje,
                    const glm::mat4& vp, float anchoEscena, float alto);
    int detectarMarcador(ImVec2 cursor) const;
};
