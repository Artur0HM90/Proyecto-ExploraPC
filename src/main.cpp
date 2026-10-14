#include "Dibujo.h"
#include "Camara.h"
#include "Componentes.h"
#include "InterfazEducativa.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "../tests/PruebaGrafica.h"

namespace {
struct Entrada { double scroll=0; std::vector<int> teclas; };
void tecla(GLFWwindow* v,int key,int,int accion,int) {
    if(accion==GLFW_PRESS) static_cast<Entrada*>(glfwGetWindowUserPointer(v))->teclas.push_back(key);
}
void comprobar(bool correcto,const char* mensaje) { if(!correcto) throw std::runtime_error(mensaje); }
void captura(const char* nombre,int ancho,int alto) {
    std::filesystem::create_directories("evidencias");
    std::vector<unsigned char> pixeles(size_t(ancho)*size_t(alto)*3);
    glPixelStorei(GL_PACK_ALIGNMENT,1); glReadBuffer(GL_BACK);
    glReadPixels(0,0,ancho,alto,GL_RGB,GL_UNSIGNED_BYTE,pixeles.data());
    std::ofstream archivo(std::filesystem::path("evidencias")/nombre,std::ios::binary);
    comprobar(bool(archivo),"No se pudo guardar la captura");
    archivo << "P6\n" << ancho << ' ' << alto << "\n255\n";
    for(int y=alto-1;y>=0;--y)
        archivo.write(reinterpret_cast<const char*>(pixeles.data()+size_t(y)*size_t(ancho)*3),std::streamsize(ancho)*3);
}
}
int main(int argc,char** argv) {
    glfwSetErrorCallback([](int,const char* mensaje) { std::cerr << mensaje << '\n'; });
    if(!glfwInit()) return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3); glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES,4);
    GLFWwindow* ventana=glfwCreateWindow(1280,900,"ExploraPC v2 - aprende como trabaja una computadora",nullptr,nullptr);
    if(!ventana) { glfwTerminate(); return 1; }
    glfwSetWindowSizeLimits(ventana,900,720,GLFW_DONT_CARE,GLFW_DONT_CARE);
    glfwMakeContextCurrent(ventana); glfwSwapInterval(1);
    Entrada entrada; glfwSetWindowUserPointer(ventana,&entrada); glfwSetKeyCallback(ventana,tecla);
    glfwSetScrollCallback(ventana,[](GLFWwindow* v,double,double y) {
        static_cast<Entrada*>(glfwGetWindowUserPointer(v))->scroll+=y;
    });
    int resultado=0;
    bool contextoUI=false,glfwUI=false,openGLUI=false;
    try {
        comprobar(gladLoadGL(reinterpret_cast<GLADloadfunc>(glfwGetProcAddress))!=0,"No se pudo cargar OpenGL 3.3");
        std::cout << "OpenGL: " << glGetString(GL_VERSION) << " | " << glGetString(GL_RENDERER) << '\n';
        float escalaX,escalaY; glfwGetWindowContentScale(ventana,&escalaX,&escalaY);
        std::cout << "Escala de pantalla observada: " << escalaX << " x " << escalaY << '\n';
        IMGUI_CHECKVERSION(); ImGui::CreateContext(); contextoUI=true;
        auto& io=ImGui::GetIO(); io.IniFilename=nullptr;
        // Fuente del sistema; la fuente integrada cubre el español si Segoe UI no existe.
        if(std::filesystem::exists("C:/Windows/Fonts/segoeui.ttf")) io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/segoeui.ttf",18);
        else { ImFontConfig config; config.SizePixels=18; io.Fonts->AddFontDefault(&config); }
        InterfazEducativa::estilo();
        glfwUI=ImGui_ImplGlfw_InitForOpenGL(ventana,true);
        comprobar(glfwUI,"No se pudo iniciar la interfaz GLFW");
        openGLUI=ImGui_ImplOpenGL3_Init("#version 330 core");
        comprobar(openGLUI,"No se pudo iniciar la interfaz OpenGL");
        Dibujo dibujo(std::filesystem::absolute(argv[0]).parent_path()/"shaders");
        Camara camara; Componentes componentes; InterfazEducativa interfaz; Aprendizaje aprendizaje;
        glEnable(GL_DEPTH_TEST); glEnable(GL_MULTISAMPLE);
        double previoTiempo=glfwGetTime(); int cuadros=0;
        bool prueba=argc>1 && std::string(argv[1])=="--smoke";
        bool completa=argc>1 && std::string(argv[1])=="--self-test";
        PruebaGrafica pruebaGrafica;
        while(!glfwWindowShouldClose(ventana)) {
            glfwPollEvents(); double ahora=glfwGetTime(); float dt=float(ahora-previoTiempo); previoTiempo=ahora;
            int ancho,alto,fbAncho,fbAlto;
            glfwGetWindowSize(ventana,&ancho,&alto); glfwGetFramebufferSize(ventana,&fbAncho,&fbAlto);
            // Al minimizar, el framebuffer puede ser cero: evitamos dividir entre cero.
            if(fbAncho<=0 || fbAlto<=0) { entrada.scroll=0; entrada.teclas.clear(); glfwWaitEventsTimeout(.05); continue; }
            ImGui_ImplOpenGL3_NewFrame(); ImGui_ImplGlfw_NewFrame();
            if(completa) {
                entrada.scroll+=pruebaGrafica.eventos(cuadros,interfaz,ventana);
                if(int key=PruebaGrafica::key(cuadros)) tecla(ventana,key,0,GLFW_PRESS,0);
            }
            ImGui::NewFrame();
            if(prueba && cuadros<5) tecla(ventana,GLFW_KEY_1+cuadros,0,GLFW_PRESS,0);
            for(int key : entrada.teclas) {
                if(key==GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(ventana,GLFW_TRUE);
                if(!io.WantCaptureKeyboard) {
                    if(key>=GLFW_KEY_1 && key<=GLFW_KEY_5) aprendizaje.seleccionar(key-GLFW_KEY_1,componentes);
                    if(key==GLFW_KEY_R) camara.restablecer();
                }
            }
            entrada.teclas.clear();
            if(prueba && cuadros<5) comprobar(componentes.seleccionado==cuadros,"Seleccion por teclas");
            float escena=float(ancho)-InterfazEducativa::anchoPanel(float(ancho));
            float x=io.MousePos.x,y=io.MousePos.y;
            bool sobreEscena=x>=0 && x<escena && y>=0 && y<alto;
            bool clic=camara.entrada(x,y,io.MouseDown[0],sobreEscena && !io.WantCaptureMouse,float(entrada.scroll));
            if(clic) aprendizaje.seleccionar(interfaz.detectarMarcador({x,y}),componentes);
            entrada.scroll=0;
            // Tiempo simulado sólo para la prueba reproducible; la aplicación usa glfwGetTime.
            componentes.actualizar(completa ? 1.f/30.f : dt);
            aprendizaje.actualizar(completa ? 1.f/30.f : dt,componentes);
            interfaz.dibujar(componentes,camara,aprendizaje,float(ancho),float(alto));
            if(completa) pruebaGrafica.verificar(cuadros,componentes,camara,aprendizaje,interfaz,ancho,alto);
            // La interfaz usa puntos; OpenGL usa píxeles del framebuffer (pantallas DPI).
            int escenaPixeles=std::max(1,int(escena*float(fbAncho)/float(ancho)));
            glViewport(0,0,fbAncho,fbAlto); glClearColor(.035f,.055f,.095f,1); glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
            glViewport(0,0,escenaPixeles,fbAlto);
            auto vp=camara.vistaProyeccion(float(escenaPixeles)/float(fbAlto));
            dibujo.comenzar(vp); dibujo.gabinete(); componentes.dibujar(dibujo,aprendizaje.verDemostracion);
            interfaz.marcadores(componentes,aprendizaje,vp,escena,float(alto));
            ImGui::Render(); glViewport(0,0,fbAncho,fbAlto); ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            comprobar(glGetError()==GL_NO_ERROR,"Error OpenGL durante el dibujo");
            if(prueba && cuadros==18) captura("comprobacion_rapida.ppm",fbAncho,fbAlto);
            if(completa && cuadros==170) captura("v2_separado.ppm",fbAncho,fbAlto);
            if(completa && cuadros==315) captura("v2_libre_960.ppm",fbAncho,fbAlto);
            if(completa && cuadros==394) captura("v2_recorrido_960.ppm",fbAncho,fbAlto);
            if(completa && cuadros==610) captura("v2_demo_ram_ensamblada.ppm",fbAncho,fbAlto);
            if(completa && cuadros==680) captura("v2_demo_ram_separada.ppm",fbAncho,fbAlto);
            if(completa && cuadros==730) captura("v2_demo_cpu_pausada.ppm",fbAncho,fbAlto);
            glfwSwapBuffers(ventana);
            ++cuadros;
            if(prueba && cuadros==20) glfwSetWindowShouldClose(ventana,GLFW_TRUE);
            if(completa && cuadros>PruebaGrafica::ultimoCuadro+5) throw std::runtime_error("Escape no cerro la ventana");
        }
        if(completa) {
            comprobar(cuadros==PruebaGrafica::ultimoCuadro+1,"Prueba grafica interrumpida");
            std::cout << "Cierre mediante Esc y liberacion de recursos: OK\n";
        }
        if(prueba || completa) {
            PruebaGrafica::comprobarLogica();
            camara.acercar(1000); comprobar(camara.distancia()==6.5f,"Limite cercano");
            camara.acercar(-1000); comprobar(camara.distancia()==20.f,"Limite lejano");
            camara.girar(2000,2000); comprobar(camara.elevacion()<=1.1f,"Limite orbital");
            camara.restablecer(); comprobar(camara.distancia()==10.8f,"Restablecer");
            for(const auto& p : componentes.piezas) comprobar(p.funcion[0] && p.comparacion[0] && p.relacion[0],"Texto educativo incompleto");
            Componentes lento,rapido;
            lento.separado=rapido.separado=true;
            for(int i=0;i<30;++i) lento.actualizar(1.f/30);
            for(int i=0;i<120;++i) rapido.actualizar(1.f/120);
            comprobar(std::abs(lento.avance-rapido.avance)<.00001f,"Animacion depende de FPS");
            lento.actualizar(1); comprobar(lento.avance==1,"Fin de separacion");
            lento.separado=false; lento.actualizar(2); comprobar(lento.avance==0,"Fin de ensamblaje");
            lento.separado=true; lento.actualizar(.4f);
            auto antes=lento.posicion(1); lento.separado=false;
            comprobar(lento.posicion(1)==antes,"Salto al invertir animacion");
            camara.entrada(1000,200,true,false,4); camara.entrada(1050,250,true,false,4);
            comprobar(camara.azimut()==.32f && camara.distancia()==10.8f,"Panel mueve camara");
            camara.entrada(100,200,false,true,0); camara.entrada(100,200,true,true,0);
            camara.entrada(150,240,true,true,1);
            comprobar(camara.azimut()!=.32f && camara.distancia()<10.8f,"Entrada de camara");
            std::cout << "Cinco selecciones por teclas, textos, camara y dibujo: correcto\n";
            std::cout << "Animacion a 30/120 FPS, ensamblaje, inversion y aislamiento del panel: correcto\n";
        }
    } catch(const std::exception& error) { std::cerr << "ExploraPC: " << error.what() << '\n'; resultado=1; }
    // Dibujo libera sus recursos antes de destruir el contexto OpenGL.
    if(openGLUI) ImGui_ImplOpenGL3_Shutdown();
    if(glfwUI) ImGui_ImplGlfw_Shutdown();
    if(contextoUI) ImGui::DestroyContext();
    glfwDestroyWindow(ventana); glfwTerminate(); return resultado;
}
