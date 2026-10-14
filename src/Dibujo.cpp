#include "Dibujo.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace {
std::string leer(const std::filesystem::path& archivo) {
    std::ifstream entrada(archivo);
    if (!entrada) throw std::runtime_error("No se pudo abrir el shader: " + archivo.string());
    std::ostringstream texto; texto << entrada.rdbuf(); return texto.str();
}
GLuint compilar(GLenum tipo, const std::string& texto) {
    GLuint shader = glCreateShader(tipo);
    const char* fuente = texto.c_str(); glShaderSource(shader, 1, &fuente, nullptr);
    glCompileShader(shader);
    GLint correcto; glGetShaderiv(shader, GL_COMPILE_STATUS, &correcto);
    if (!correcto) {
        char mensaje[2048]; glGetShaderInfoLog(shader, sizeof(mensaje), nullptr, mensaje);
        glDeleteShader(shader); throw std::runtime_error(mensaje);
    }
    return shader;
}
}
Dibujo::Dibujo(const std::filesystem::path& shaders) {
    GLuint vertice = compilar(GL_VERTEX_SHADER, leer(shaders / "pieza.vert"));
    GLuint fragmento = 0;
    try { fragmento = compilar(GL_FRAGMENT_SHADER, leer(shaders / "pieza.frag")); }
    catch (...) { glDeleteShader(vertice); throw; }
    programa_ = glCreateProgram(); glAttachShader(programa_, vertice); glAttachShader(programa_, fragmento);
    glLinkProgram(programa_); glDeleteShader(vertice); glDeleteShader(fragmento);
    GLint correcto; glGetProgramiv(programa_, GL_LINK_STATUS, &correcto);
    if (!correcto) {
        char mensaje[2048]; glGetProgramInfoLog(programa_, sizeof(mensaje), nullptr, mensaje);
        glDeleteProgram(programa_); throw std::runtime_error(mensaje);
    }
    uVP_ = glGetUniformLocation(programa_, "vistaProyeccion");
    uModelo_ = glGetUniformLocation(programa_, "modelo");
    uColor_ = glGetUniformLocation(programa_, "colorPieza");
    uResaltado_ = glGetUniformLocation(programa_, "resaltado");
    uTextura_ = glGetUniformLocation(programa_, "textura");
    // Cada cara tiene su propia normal: así los bordes del cubo se ven definidos.
    std::vector<float> vertices;
    const glm::vec3 normales[] = {{1,0,0},{-1,0,0},{0,1,0},{0,-1,0},{0,0,1},{0,0,-1}};
    for (glm::vec3 n : normales) {
        glm::vec3 u = (n.y != 0) ? glm::vec3(1,0,0) : glm::vec3(0,1,0);
        glm::vec3 v = glm::cross(n,u);
        glm::vec3 puntos[] = {n*.5f-u*.5f-v*.5f, n*.5f+u*.5f-v*.5f,
                             n*.5f+u*.5f+v*.5f, n*.5f-u*.5f+v*.5f};
        for (int i : {0,1,2,0,2,3}) {
            auto p = puntos[i];
            vertices.insert(vertices.end(), {p.x,p.y,p.z,n.x,n.y,n.z});
        }
    }
    glGenVertexArrays(1,&vao_); glGenBuffers(1,&vbo_); glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER,vbo_);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size()*sizeof(float)), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),nullptr); glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),reinterpret_cast<void*>(3*sizeof(float))); glEnableVertexAttribArray(1);
}
Dibujo::~Dibujo() {
    glDeleteBuffers(1,&vbo_); glDeleteVertexArrays(1,&vao_); glDeleteProgram(programa_);
}
void Dibujo::comenzar(const glm::mat4& vp) {
    glUseProgram(programa_); glBindVertexArray(vao_);
    glUniformMatrix4fv(uVP_,1,GL_FALSE,glm::value_ptr(vp));
}
void Dibujo::caja(glm::vec3 posicion, glm::vec3 tamano, glm::vec3 color, bool resaltado, int textura) {
    // Modelo = traslación * escala. La escala actúa primero sobre el cubo unitario.
    glm::mat4 modelo = glm::scale(glm::translate(glm::mat4(1),posicion),tamano);
    glUniformMatrix4fv(uModelo_,1,GL_FALSE,glm::value_ptr(modelo));
    glUniform3fv(uColor_,1,glm::value_ptr(color)); glUniform1i(uResaltado_,resaltado);
    glUniform1i(uTextura_,textura); glDrawArrays(GL_TRIANGLES,0,36);
}
void Dibujo::gabinete() {
    const glm::vec3 metal(.23f,.30f,.40f);
    // Estructura abierta: no hay una tapa opaca que esconda las piezas.
    caja({0,-2.4f,0},{3.6f,.14f,1.9f},metal);
    for (float x : {-1.75f,1.75f}) {
        for (float z : {-.9f,.9f}) caja({x,0,z},{.09f,4.8f,.09f},metal);
        caja({x,2.35f,0},{.09f,.09f,1.9f},metal);
    }
    for (float z : {-.9f,.9f}) caja({0,2.35f,z},{3.6f,.09f,.09f},metal);
    caja({0,-2.55f,0},{6.8f,.10f,5.5f},{.10f,.14f,.22f});
}
