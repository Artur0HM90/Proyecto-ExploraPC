#version 330 core
layout(location = 0) in vec3 posicion;
layout(location = 1) in vec3 normal;
uniform mat4 modelo;
uniform mat4 vistaProyeccion;
out vec3 normalMundo;
out vec3 posicionLocal;
void main() {
    gl_Position = vistaProyeccion * modelo * vec4(posicion, 1.0);
    // La inversa transpuesta conserva normales correctas con escalas distintas.
    normalMundo = mat3(transpose(inverse(modelo))) * normal;
    posicionLocal = posicion;
}
