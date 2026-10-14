# Dependencias incluidas

La carpeta inicial estaba vacía. No había una base del profesor ni un cargador previo. Se incluyeron fuentes de versiones concretas, sin modificar su código.

| Biblioteca | Versión | Función | Procedencia | Licencia |
|---|---|---|---|---|
| GLFW | 3.4 | Ventana, contexto y eventos | [Repositorio oficial](https://github.com/glfw/glfw/tree/3.4) | third_party/glfw/LICENSE.md; zlib/libpng |
| GLM | 1.0.1 | Vectores y matrices | [Repositorio oficial](https://github.com/g-truc/glm/tree/1.0.1) | third_party/glm/copying.txt; MIT / Happy Bunny |
| Dear ImGui | 1.91.9b | Panel educativo | [Repositorio oficial](https://github.com/ocornut/imgui/tree/v1.91.9b) | third_party/imgui/LICENSE.txt; MIT |
| GLAD | Generador 2.0.8 | Funciones OpenGL 3.3 core | [Repositorio oficial](https://github.com/Dav1dde/glad/tree/v2.0.8) | Avisos en los archivos generados |

GLAD se generó para gl:core=3.3, sin extensiones, usando la especificación incluida en el generador con --reproducible. Se incluyen src/gl.c, include/glad/gl.h e include/KHR/khrplatform.h. No hace falta regenerarlo. Se carga mediante glfwGetProcAddress. El backend de ImGui emplea su cargador interno, aislado en su unidad de compilación.

Comando de generación, sólo como referencia de mantenimiento:

```powershell
python -m glad --api gl:core=3.3 --extensions "" --out-path third_party/glad --reproducible c
```

SHA-256 de los ZIP descargados de repositorios oficiales:

```text
glfw 3.4.zip:
A133DDC3D3C66143EBA9035621DB8E0BCF34DBA1EE9514A9E23E96AFD39FD57A
glm 1.0.1.zip:
09C5716296787E1F7FCB87B1CBDBF26814EC1288ED6259CCD30D5D9795809FA5
imgui v1.91.9b.zip:
FD37507C8476A6D14CC7C4B352401F31BCBD0F0D995D35390811E968C466F46E
```

Segoe UI se lee desde Windows cuando existe; no se redistribuye. La fuente integrada de ImGui actúa como alternativa. Los shaders y patrones propios están en shaders. Python y el generador se usaron para preparar el proyecto y no son requisitos para compilarlo.
