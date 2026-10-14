#pragma once
#include <filesystem>
#include <glad/gl.h>
#include <glm/glm.hpp>

// Un solo cubo se reutiliza mediante transformaciones para construir toda la PC.
class Dibujo {
public:
    explicit Dibujo(const std::filesystem::path& shaders);
    ~Dibujo();
    Dibujo(const Dibujo&) = delete;
    Dibujo& operator=(const Dibujo&) = delete;
    void comenzar(const glm::mat4& vistaProyeccion);
    void caja(glm::vec3 posicion, glm::vec3 tamano, glm::vec3 color,
              bool resaltado = false, int textura = 0);
    void gabinete();
private:
    GLuint programa_ = 0, vao_ = 0, vbo_ = 0;
    GLint uVP_, uModelo_, uColor_, uResaltado_, uTextura_;
};
