#include "InterfazEducativa.h"
#include <algorithm>
#include <cmath>
#include <string>

namespace {
constexpr ImU32 datos=IM_COL32(79,231,224,255), energia=IM_COL32(255,202,85,255);
ImVec2 centro() {
    auto a=ImGui::GetItemRectMin(),b=ImGui::GetItemRectMax();
    return {(a.x+b.x)*.5f,(a.y+b.y)*.5f};
}
bool boton(const char* nombre,ImVec2& posicion,ImVec2 tamano={-1,30}) {
    bool pulsado=ImGui::Button(nombre,tamano); posicion=centro(); return pulsado;
}
void parrafo(const char* titulo,const char* texto) {
    ImGui::TextColored({.43f,.84f,.76f,1},"%s",titulo);
    ImGui::PushTextWrapPos(0); ImGui::TextUnformatted(texto); ImGui::PopTextWrapPos(); ImGui::Spacing();
}
bool proyectar(glm::vec3 punto,const glm::mat4& vp,float ancho,float alto,ImVec2& pantalla) {
    glm::vec4 clip=vp*glm::vec4(punto,1);
    if(clip.w<=0) return false;
    auto ndc=glm::vec3(clip)/clip.w;
    if(ndc.z < -1 || ndc.z > 1) return false;
    pantalla={(ndc.x+1)*.5f*ancho,(1-ndc.y)*.5f*alto}; return true;
}
void flecha(ImDrawList* lista,ImVec2 a,ImVec2 b,ImU32 tono) {
    float dx=b.x-a.x,dy=b.y-a.y,l=std::sqrt(dx*dx+dy*dy); if(l<1) return;
    dx/=l; dy/=l; ImVec2 punta{a.x+(b.x-a.x)*.85f,a.y+(b.y-a.y)*.85f};
    lista->AddLine(a,b,tono,2);
    lista->AddTriangleFilled(punta,{punta.x-dx*10-dy*5,punta.y-dy*10+dx*5},
                                  {punta.x-dx*10+dy*5,punta.y-dy*10-dx*5},tono);
}
}
void InterfazEducativa::estilo() {
    ImGui::StyleColorsDark(); auto& s=ImGui::GetStyle();
    s.WindowPadding={14,14}; s.FramePadding={9,5}; s.ItemSpacing={6,6};
    s.WindowRounding=0; s.FrameRounding=5; s.ScrollbarSize=13;
    s.Colors[ImGuiCol_WindowBg]={.065f,.09f,.14f,1};
    s.Colors[ImGuiCol_Button]={.14f,.20f,.29f,1};
    s.Colors[ImGuiCol_ButtonHovered]={.21f,.31f,.43f,1};
    s.Colors[ImGuiCol_ButtonActive]={.19f,.40f,.42f,1};
    s.Colors[ImGuiCol_Text]={.91f,.94f,.98f,1};
}
float InterfazEducativa::anchoPanel(float ancho) { return std::clamp(ancho*.33f,360.f,430.f); }
void InterfazEducativa::dibujar(Componentes& c,Camara& camara,Aprendizaje& a,float ancho,float alto) {
    float panel=anchoPanel(ancho),escena=ancho-panel;
    ImGui::SetNextWindowPos({escena,0}); ImGui::SetNextWindowSize({panel,alto});
    ImGui::Begin("Aprende sobre tu PC",nullptr,ImGuiWindowFlags_NoTitleBar|ImGuiWindowFlags_NoMove
        |ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoSavedSettings|ImGuiWindowFlags_NoScrollbar);
    ImGui::SetWindowFontScale(1.25f); ImGui::TextUnformatted("ExploraPC · versión 2"); ImGui::SetWindowFontScale(1);
    auto modoBoton=[&](const char* titulo,ImVec2& posicion,bool activo) {
        if(activo) ImGui::PushStyleColor(ImGuiCol_Button,{.17f,.35f,.37f,1});
        bool clic=boton(titulo,posicion);
        if(activo) ImGui::PopStyleColor();
        return clic;
    };
    if(modoBoton("Explorar libremente",controles.libre,a.modo==Modo::Libre && !a.verDemostracion)) a.explorar();
    if(modoBoton("Aprender paso a paso",controles.guiado,a.modo==Modo::Guiado)) a.guiar(c);
    if(modoBoton("¿Qué pasa cuando abro un programa?",controles.abrirDemo,a.verDemostracion)) a.abrirDemostracion(c);
    ImGui::Separator();

    // Área educativa flexible; sólo la lectura se desplaza. Navegación y acciones
    // permanecen visibles incluso a 960 x 720.
    float alturaCuerpo=std::max(250.f,ImGui::GetContentRegionAvail().y-123.f);
    ImGui::BeginChild("Contenido",{0,alturaCuerpo},ImGuiChildFlags_None,ImGuiWindowFlags_NoScrollbar);
    if(a.verDemostracion) {
        ImGui::TextUnformatted("Demostración didáctica simplificada");
        if(boton("Reproducir",controles.reproducir)) a.reproducir();
        float mitad=(ImGui::GetContentRegionAvail().x-6)*.5f;
        ImGui::BeginDisabled(!a.iniciada || a.tiempo>=Aprendizaje::duracion);
        if(boton(a.reproduciendo ? "Pausar" : "Continuar",controles.pausa,{mitad,30})) a.pausarContinuar();
        ImGui::EndDisabled(); ImGui::SameLine();
        if(boton("Reiniciar",controles.reiniciar,{mitad,30})) a.reiniciar();
        ImGui::ProgressBar(a.tiempo/Aprendizaje::duracion,{-1,8},"");
        ImGui::TextColored({.65f,.75f,.85f,1},"%s",a.tiempo>=Aprendizaje::duracion ? "Finalizada · puedes reproducir otra vez" :
            a.reproduciendo ? "Reproduciendo" : a.iniciada ? "En pausa" : "Lista para reproducir");
        ImGui::TextColored({.43f,.84f,.76f,1},"RAM: temporal · SSD: persistente");
    } else if(a.modo==Modo::Guiado) {
        float mitad=(ImGui::GetContentRegionAvail().x-6)*.5f;
        ImGui::BeginDisabled(a.paso==0);
        if(boton("Anterior",controles.anterior,{mitad,30})) a.avanzar(-1,c);
        ImGui::EndDisabled(); ImGui::SameLine(); ImGui::BeginDisabled(a.paso==4);
        if(boton("Siguiente",controles.siguiente,{mitad,30})) a.avanzar(1,c);
        ImGui::EndDisabled();
        if(boton("Volver a explorar",controles.volver)) a.explorar();
        ImGui::TextColored({.43f,.84f,.76f,1},"Paso %d de 5",a.paso+1);
    } else {
        const char* nombres[]={"1  Placa madre","2  Procesador","3  Memoria RAM","4  Unidad SSD","5  Fuente"};
        float mitad=(ImGui::GetContentRegionAvail().x-6)*.5f;
        for(int i=0;i<5;++i) {
            ImGui::PushID(i); bool activo=i==c.seleccionado;
            if(activo) ImGui::PushStyleColor(ImGuiCol_Button,{.17f,.35f,.37f,1});
            if(boton(nombres[i],controles.piezas[i],{i==4 ? -1.f : mitad,30})) a.seleccionar(i,c);
            if(activo) ImGui::PopStyleColor(); ImGui::PopID();
            if(i==0 || i==2) ImGui::SameLine();
        }
    }
    a.actualizar(0,c); // Sincroniza también después de un botón de reinicio o navegación.
    ImGui::TextColored({.65f,.75f,.85f,1},"Lectura: rueda o barra lateral");
    ImGui::BeginChild("Lectura",{0,0},ImGuiChildFlags_Borders,ImGuiWindowFlags_AlwaysVerticalScrollbar);
    if(a.verDemostracion) {
        parrafo(a.tituloFase(),a.explicacionFase());
        parrafo("Energía durante todo el proceso","La fuente suministra energía al sistema (línea dorada discontinua). La placa madre permite que las piezas se comuniquen.");
        parrafo("Información, no electricidad","Los cuadrados turquesa representan instrucciones y datos. Las flechas muestran su recorrido, no cables físicos.");
        parrafo("Al apagar","La RAM pierde su contenido; el SSD conserva sus archivos. Esta secuencia es una simplificación para aprender.");
    } else {
        auto& pieza=c.piezas[c.seleccionado];
        parrafo(pieza.nombre,pieza.funcion);
        parrafo("Comparación cotidiana",pieza.comparacion);
        parrafo("Relación con las demás piezas",pieza.relacion);
    }
    ImGui::EndChild(); ImGui::EndChild();
    ImGui::Separator();
    if(boton("Separar componentes",controles.separar)) c.separado=true;
    float mitad=(ImGui::GetContentRegionAvail().x-6)*.5f;
    if(boton("Ensamblar",controles.ensamblar,{mitad,30})) c.separado=false;
    ImGui::SameLine();
    if(boton("Restablecer vista",controles.restablecer,{mitad,30})) camara.restablecer();
    ImGui::TextColored({.65f,.75f,.85f,1},"%s",c.avance>.001f && c.avance<.999f ? "Moviendo las piezas..." : c.separado ? "Vista separada" : "Vista ensamblada");
    ImGui::End();

    auto* fondo=ImGui::GetBackgroundDrawList();
    fondo->AddText({20,20},IM_COL32(99,220,190,255),"EXPLORAPC · APRENDE CÓMO TRABAJA UNA PC");
    if(a.verDemostracion) {
        fondo->AddText({20,47},IM_COL32(235,243,255,255),a.tituloFase());
        fondo->AddRectFilled({20,82},{30,92},datos);
        fondo->AddText({38,76},datos,"Datos (cuadrados)");
        for(int x=216;x<246;x+=10) fondo->AddLine({float(x),87},{float(x+5),87},energia,2);
        fondo->AddText({254,76},energia,"Energía (discontinua)");
    } else {
        std::string titulo=a.modo==Modo::Guiado ? "Paso "+std::to_string(a.paso+1)+" de 5 · "+c.piezas[c.seleccionado].nombre :
            "Exploración libre · elige un marcador";
        fondo->AddText({20,51},IM_COL32(235,243,255,255),titulo.c_str());
    }
    fondo->AddText({20,alto-56},IM_COL32(154,177,204,255),"Arrastrar: girar   |   Rueda: zoom   |   R: vista inicial");
    fondo->AddText({20,alto-30},IM_COL32(154,177,204,255),a.modo==Modo::Guiado ?
        "Recorrido: Anterior / Siguiente en el panel   |   Esc: salir" :
        "Clic en número o 1-5: explorar una pieza   |   Esc: salir");
}
int InterfazEducativa::detectarMarcador(ImVec2 cursor) const {
    int elegido=-1; float menor=18.f*18.f;
    for(int i=0;i<5;++i) if(marcadoresVisibles[i]) {
        float dx=cursor.x-centrosMarcadores[i].x,dy=cursor.y-centrosMarcadores[i].y;
        float distancia=dx*dx+dy*dy;
        if(distancia<menor) { menor=distancia; elegido=i; }
    }
    return elegido;
}
void InterfazEducativa::marcadores(const Componentes& c,const Aprendizaje& a,const glm::mat4& vp,float ancho,float alto) {
    auto* lista=ImGui::GetBackgroundDrawList(); lista->PushClipRect({0,110},{ancho,alto-78},true);
    if(a.verDemostracion) {
        ImVec2 fuente,placa;
        if(proyectar(c.marcador(4),vp,ancho,alto,fuente) && proyectar(c.marcador(0),vp,ancho,alto,placa)) {
            // Energía: línea discontinua fija, distinta de las flechas de datos.
            for(int i=0;i<20;i+=2) {
                float t=float(i)/20.f,u=float(i+1)/20.f;
                lista->AddLine({fuente.x+(placa.x-fuente.x)*t,fuente.y+(placa.y-fuente.y)*t},
                               {fuente.x+(placa.x-fuente.x)*u,fuente.y+(placa.y-fuente.y)*u},energia,2);
            }
            lista->AddText({fuente.x-20,fuente.y+22},energia,"ENERGÍA");
            lista->AddText({placa.x-20,placa.y-40},datos,"COMUNICACIÓN");
        }
        if(a.fase()>0) {
            auto ruta=a.rutaDatos(c); ImVec2 puntos[3];
            if(proyectar(ruta[0],vp,ancho,alto,puntos[0]) && proyectar(ruta[1],vp,ancho,alto,puntos[1])
                && proyectar(ruta[2],vp,ancho,alto,puntos[2])) {
                flecha(lista,puntos[0],puntos[1],datos); flecha(lista,puntos[1],puntos[2],datos);
            }
        }
        for(int i=0;i<3;++i) {
            ImVec2 p;
            if(proyectar(a.posicionDato(c,i),vp,ancho,alto,p)) {
                float radio=a.fase()==0 ? 5.f+std::sin(a.tiempo*3.f+float(i)) : 5.f;
                lista->AddRectFilled({p.x-radio,p.y-radio},{p.x+radio,p.y+radio},datos,1);
            }
        }
    }
    for(int i=0;i<5;++i) {
        ImVec2 pos;
        bool proyectado=proyectar(c.marcador(i),vp,ancho,alto,pos);
        centrosMarcadores[i]=pos;
        // Sólo se puede pulsar la parte visible de la escena, no el panel.
        marcadoresVisibles[i]=proyectado && pos.x>=18 && pos.x<ancho-18 && pos.y>=128 && pos.y<alto-96;
        if(!proyectado) continue;
        auto color=c.piezas[i].color; ImU32 tono=ImGui::ColorConvertFloat4ToU32({color.r,color.g,color.b,1});
        lista->AddCircleFilled(pos,16,IM_COL32(10,18,28,245));
        lista->AddCircle(pos,16,tono,24,i==c.seleccionado ? 3.f : 1.5f);
        char numero[2]={char('1'+i),0}; auto tamano=ImGui::CalcTextSize(numero);
        lista->AddText({pos.x-tamano.x*.5f,pos.y-tamano.y*.5f},IM_COL32(255,255,255,255),numero);
        if(i==c.seleccionado) {
            const char* nombre=c.piezas[i].nombre; auto medida=ImGui::CalcTextSize(nombre);
            float x=std::clamp(pos.x+23,4.f,std::max(4.f,ancho-medida.x-12));
            ImVec2 texto{x,pos.y+20};
            lista->AddRectFilled({texto.x-5,texto.y-3},{texto.x+medida.x+5,texto.y+medida.y+3},IM_COL32(10,18,28,240),4);
            lista->AddText(texto,IM_COL32(242,246,255,255),nombre);
        }
    }
    lista->PopClipRect();
}
