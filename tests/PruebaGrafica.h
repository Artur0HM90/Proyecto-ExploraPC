#pragma once
#include "InterfazEducativa.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <stdexcept>

// Prueba de extremo a extremo: los botones reciben eventos de ImGui.
// La lógica educativa también tiene comprobaciones independientes al finalizar.
class PruebaGrafica {
public:
    static constexpr int ultimoCuadro=970;
    static int key(int cuadro) {
        if(cuadro>=10 && cuadro<=26 && (cuadro-10)%4==0) return GLFW_KEY_1+(cuadro-10)/4;
        if(cuadro==70) return GLFW_KEY_2;
        if(cuadro==365) return GLFW_KEY_5; // Debe ignorarse en el recorrido.
        if(cuadro==ultimoCuadro) return GLFW_KEY_ESCAPE;
        return 0;
    }
    float eventos(int f,const InterfazEducativa& ui,GLFWwindow* ventana) {
        const auto& b=ui.controles; float rueda=0;
        if(f>=40 && f<70) clic((f-40)%6,b.piezas[(f-40)/6]);
        programar(f,74,b.separar); programar(f,180,b.ensamblar);
        if(f==184) posicion_={200,200};
        if(f==185) pulsado_=true;
        if(f==188) posicion_={260,240};
        if(f==189) rueda=1;
        if(f==190) pulsado_=false;
        programar(f,195,b.restablecer);
        if(f==305) glfwSetWindowSize(ventana,960,720);
        programar(f,320,ui.centrosMarcadores[2]);
        // Un arrastre que empieza sobre el SSD debe girar, sin seleccionar.
        if(f==330) posicion_=ui.centrosMarcadores[3];
        if(f==331) pulsado_=true;
        if(f==332) { posicion_.x+=38; posicion_.y+=20; }
        if(f==333) pulsado_=false;
        programar(f,340,b.restablecer);
        programar(f,350,b.guiado); programar(f,355,b.anterior);
        for(int inicio : {360,370,380,390,400}) programar(f,inicio,b.siguiente);
        programar(f,410,b.anterior); programar(f,420,b.volver);
        programar(f,430,b.abrirDemo); programar(f,440,b.reproducir);
        programar(f,460,b.pausa); programar(f,480,b.pausa);
        programar(f,620,b.separar);
        programar(f,720,b.pausa); programar(f,740,b.pausa);
        programar(f,890,b.reiniciar); programar(f,900,b.reproducir);
        programar(f,910,b.guiado); programar(f,920,b.libre);
        for(int i=0;i<5;++i) programar(f,930+i*6,ui.centrosMarcadores[i]);
        auto& io=ImGui::GetIO();
        io.AddMousePosEvent(posicion_.x,posicion_.y); io.AddMouseButtonEvent(0,pulsado_);
        return rueda;
    }
    void verificar(int f,const Componentes& c,const Camara& camara,const Aprendizaje& a,
                   const InterfazEducativa& ui,int ancho,int alto) {
        if(f>=12 && f<=28 && (f-12)%4==0) {
            int i=(f-12)/4; exigir(c.seleccionado==i,"Tecla 1-5");
            std::cout << "Tecla " << i+1 << ": OK\n";
        }
        if(f>=43 && f<70 && (f-43)%6==0) {
            int i=(f-43)/6; exigir(c.seleccionado==i,"Boton de componente");
            exigir(camara.azimut()==.32f && camara.distancia()==10.8f,"Panel mueve camara");
            std::cout << "Boton " << i+1 << ": OK\n";
        }
        if(f==100) exigir(c.separado && c.avance>0 && c.avance<1,"Separacion no anima");
        if(f==170) { exigir(c.avance==1,"Separacion incompleta"); std::cout << "Separar: OK\n"; }
        if(f==191) exigir(camara.azimut()!=.32f && camara.distancia()<10.8f,"Arrastre/rueda");
        if(f==198) exigir(camara.azimut()==.32f && camara.distancia()==10.8f,"Restablecer");
        if(f==280) { exigir(!c.separado && c.avance==0,"Ensamblaje"); std::cout << "Ensamblar/camara: OK\n"; }
        if(f==316) { exigir(ancho==960 && alto==720,"Redimensionamiento"); std::cout << "960x720: OK\n"; }
        if(f==324) { exigir(c.seleccionado==2,"Clic en marcador"); std::cout << "Seleccion por marcador: OK\n"; }
        if(f==334) {
            exigir(c.seleccionado==2 && camara.azimut()!=.32f,"Arrastre selecciona accidentalmente");
            std::cout << "Arrastre sobre marcador sin seleccion: OK\n";
        }
        if(f==354 || f==359) exigir(a.paso==0 && c.seleccionado==4,"Limite inicial del recorrido");
        if(f==364 || f==366) exigir(a.paso==1 && c.seleccionado==0,"Paso placa / teclado durante guia");
        if(f==374) exigir(a.paso==2 && c.seleccionado==3,"Paso SSD");
        if(f==384 || f==414) exigir(a.paso==3 && c.seleccionado==2,"Paso RAM");
        if(f==394 || f==404) exigir(a.paso==4 && c.seleccionado==1,"Limite final del recorrido");
        if(f==424) { exigir(a.modo==Modo::Libre && !a.verDemostracion,"Volver a explorar"); std::cout << "Recorrido y limites: OK\n"; }
        if(a.modo==Modo::Guiado) exigir(c.seleccionado==Aprendizaje::orden[a.paso],"Texto/resaltado del paso");
        if(a.verDemostracion) exigir(c.seleccionado==a.componenteFase(),"Texto/resaltado de la fase");
        if(f==434) exigir(a.verDemostracion && a.tiempo==0 && !a.reproduciendo,"Abrir demo");
        if(f==450) exigir(a.reproduciendo && a.tiempo>0 && a.fase()==0,"Reproducir demo");
        if(f==464 || f==724) { exigir(!a.reproduciendo,"Pausar"); tiempoPausa_=a.tiempo; }
        if(f==474 || f==734) exigir(a.tiempo==tiempoPausa_,"El tiempo cambia durante pausa");
        if(f==490) exigir(a.reproduciendo && a.tiempo>tiempoPausa_,"Continuar");
        if(f==610) exigir(a.fase()==1 && c.avance==0,"Datos SSD-RAM ensamblados");
        if(f==680) exigir(a.fase()==1 && c.avance==1,"Datos SSD-RAM separados");
        if(f==730) exigir(a.fase()==2 && c.seleccionado==1,"CPU / RAM");
        if(f==880) {
            exigir(a.tiempo==Aprendizaje::duracion && !a.reproduciendo,"Final de demo");
            std::cout << "Demo: fases, pausa y continuar; ensamblada/separada: OK\n";
        }
        if(f==894) exigir(a.tiempo==0 && !a.iniciada && c.seleccionado==3,"Reiniciar demo");
        if(f==914) exigir(a.modo==Modo::Guiado && !a.verDemostracion && a.tiempo==0 && c.seleccionado==4,"Cambio de modo durante demo");
        if(f==924) { exigir(a.modo==Modo::Libre,"Explorar libremente"); std::cout << "Reinicio y cambio de modo: OK\n"; }
        if(f>=933 && f<=957 && (f-933)%6==0) {
            int i=(f-933)/6; exigir(c.seleccionado==i,"Seleccion de cada marcador");
            std::cout << "Marcador " << i+1 << ": OK\n";
        }
        if(ancho==960 && alto==720) {
            const auto& b=ui.controles;
            for(ImVec2 p : {b.libre,b.guiado,b.abrirDemo,b.separar,b.ensamblar,b.restablecer})
                exigir(p.x>600 && p.x<960 && p.y>0 && p.y<720,"Control fuera de la ventana");
            if(a.verDemostracion) for(ImVec2 p : {b.reproducir,b.pausa,b.reiniciar})
                exigir(p.y>0 && p.y<720,"Control de demo fuera de la ventana");
        }
    }
    static void comprobarLogica() {
        Componentes piezas; Aprendizaje a; a.guiar(piezas);
        a.avanzar(-1,piezas); exigir(a.paso==0,"Guia limite negativo");
        for(int i=1;i<5;++i) { a.avanzar(1,piezas); exigir(piezas.seleccionado==Aprendizaje::orden[i],"Orden de guia"); }
        a.avanzar(1,piezas); exigir(a.paso==4,"Guia limite superior");
        a.abrirDemostracion(piezas); a.reproducir(); a.actualizar(6,piezas);
        exigir(a.fase()==1 && piezas.seleccionado==2,"Fase RAM");
        a.pausarContinuar(); auto dato=a.posicionDato(piezas,0);
        a.actualizar(20,piezas); exigir(a.tiempo==6 && a.posicionDato(piezas,0)==dato,"Pausa de simbolos");
        piezas.avance=1; exigir(glm::length(a.posicionDato(piezas,0)-dato)>.1f,"Ruta ignora posiciones separadas");
        a.pausarContinuar(); a.actualizar(100,piezas); exigir(a.tiempo==12 && !a.reproduciendo && a.fase()==2,"Final acotado");
        a.reiniciar(); a.actualizar(0,piezas); exigir(a.tiempo==0 && piezas.seleccionado==3,"Reinicio");
        a.reproducir(); a.actualizar(1,piezas); a.guiar(piezas);
        exigir(a.tiempo==0 && !a.reproduciendo && piezas.seleccionado==4,"Cambio de modo");
        a.abrirDemostracion(piezas); a.reproducir(); a.seleccionar(2,piezas);
        exigir(!a.verDemostracion && piezas.seleccionado==2 && a.tiempo==0,"Seleccion libre abandona demo");
        Aprendizaje lento,rapido; lento.abrirDemostracion(piezas); rapido.abrirDemostracion(piezas);
        lento.reproducir(); rapido.reproducir();
        for(int i=0;i<90;++i) lento.actualizar(1.f/30,piezas);
        for(int i=0;i<360;++i) rapido.actualizar(1.f/120,piezas);
        exigir(std::abs(lento.tiempo-rapido.tiempo)<.0001f,"Demo depende de FPS");
        Camara camara;
        exigir(!camara.entrada(100,100,true,true,0),"Clic prematuro");
        exigir(camara.entrada(102,102,false,true,0) && camara.azimut()==.32f,"Pequeno movimiento debe ser clic");
        camara.entrada(100,100,true,true,0); camara.entrada(130,100,true,true,0);
        exigir(!camara.entrada(100,100,false,true,0),"Arrastre vuelve al origen y selecciona");
        std::cout << "Logica: guia, demo 30/120 FPS, pausa, rutas actuales y clic/arrastre: OK\n";
    }
private:
    ImVec2 posicion_{50,130}; bool pulsado_=false; float tiempoPausa_=0;
    void clic(int fase,ImVec2 posicion) {
        posicion_=posicion;
        if(fase==1) pulsado_=true;
        if(fase==2) pulsado_=false;
    }
    void programar(int cuadro,int inicio,ImVec2 posicion) {
        if(cuadro>=inicio && cuadro<=inicio+3) clic(cuadro-inicio,posicion);
    }
    static void exigir(bool correcto,const char* mensaje) { if(!correcto) throw std::runtime_error(mensaje); }
};
