# COMPUTACIÓN GRÁFICA Y VISUAL - 226534.8751 (Remoto)
## Integrantes 
|Nombres| codigo| ID GitHub|
|-------|--------|---------|
|1. Bruce Barrera Neyra|N00439177| [Brucebn89](https://github.com/Brucebn89)|
|2. Arturo Hurtado Mendez|N00438882|[Artur0HM90](https://github.com/Artur0HM90)|
|3. Alejandro Sebastian Felix Quispe|N00487581|[]()|
|4. Gonzalo Gabriel Cuba Sullca|N00417865|[]()|
|5. Jesús Alberto García Aguilar|N00383896|[]()|





# ExploraPC v2: aprende cómo funciona una computadora

Aplicación educativa de escritorio en C++17 y OpenGL 3.3 para Computación Gráfica y Visual. La versión 2 añade un recorrido guiado, una demostración de cómo se abre un programa y selección mediante los marcadores de la escena. Conserva las cinco piezas, la cámara orbital y la separación/ensamblaje de la versión anterior.

Es un modelo didáctico: colores, dimensiones y desplazamientos facilitan la identificación. No representa un montaje físico a escala ni simula el funcionamiento eléctrico.

## Requisitos

- Windows 10/11 de 64 bits.
- Visual Studio con **Desarrollo para el escritorio con C++**: MSVC x64, SDK de Windows y herramientas CMake de C++.
- CMake compatible con Visual Studio. Aquí se utilizó CMake 4.3.1 incluido con Visual Studio 2026. El preset de Visual Studio 2022 requiere CMake 3.24 o superior.
- GPU y controlador con soporte **OpenGL 3.3 core**. OpenGL lo proporciona el controlador gráfico.

Las fuentes de GLFW, GLAD, GLM y Dear ImGui ya están en `third_party`. **Compilar no requiere internet, Python, pip ni vcpkg.** Conserva esta carpeta y sus licencias al compartir el proyecto.

Si falta C++: **Visual Studio Installer → Modificar → Desarrollo para el escritorio con C++**. Incluye MSVC x64/x86, un SDK de Windows y **Herramientas de CMake de C++ para Windows**. En este equipo ya estaban instaladas.

## Ejecutar la versión compilada

Abre `build/vs2026/Release/ExploraPC.exe`. La carpeta `shaders` debe permanecer junto al ejecutable.

Para compartir sin compilar, usa **ExploraPC-v2-ejecutable.zip**: extrae todo su contenido y abre `ExploraPC-v2.exe`. Para estudiar o compilar en otro equipo, extrae **ExploraPC-v2-fuentes.zip** en una carpeta y ajusta la ruta de los comandos siguientes. Las dependencias y licencias están incluidas; los paquetes de v1 se conservan con sus nombres originales.

Desde PowerShell:

```powershell
cd C:\proeyectos\ExploraPC
.\ejecutar.ps1
```

## Compilar y abrir en Visual Studio

1. Abre **Developer PowerShell for VS 2026** desde el menú Inicio.
2. Ejecuta:

```powershell
cd C:\proeyectos\ExploraPC
cmake --preset vs2026
cmake --build --preset release --parallel 4
.\build\vs2026\Release\ExploraPC.exe
```

3. Para estudiar y depurar, abre `build/vs2026/ExploraPC.slnx` con Visual Studio. Selecciona **ExploraPC** como proyecto de inicio, configuración **Debug**, plataforma **x64** y pulsa F5. CMake establece ese proyecto por defecto para soluciones nuevas. Si Visual Studio intenta ejecutar `ALL_BUILD`, haz clic derecho sobre **ExploraPC → Establecer como proyecto de inicio**; Visual Studio puede conservar la selección anterior en una solución ya abierta.

Para Visual Studio 2022, usa Developer PowerShell for VS 2022:

```powershell
cmake --preset vs2022
cmake --build --preset release2022 --parallel 4
.\build\vs2022\Release\ExploraPC.exe
```

El preset 2022 está preparado; se verificó únicamente Visual Studio 2026.

Si CMake no está en el PATH, `compilar.ps1` localiza el CMake de Visual Studio mediante vswhere:

```powershell
.\compilar.ps1
.\ejecutar.ps1
```

Si la política de PowerShell impide ejecutar scripts, usa los comandos CMake en Developer PowerShell. No necesitas cambiar la política del sistema.

## Controles

| Acción | Control |
|---|---|
| Elegir una pieza en exploración libre | Botones, teclas **1–5** o clic en su marcador numerado |
| Iniciar recorrido | **Aprender paso a paso** |
| Cambiar paso | **Anterior / Siguiente** (limitados a los pasos 1–5) |
| Salir del recorrido | **Volver a explorar** o **Explorar libremente** |
| Abrir la demostración | **¿Qué pasa cuando abro un programa?** |
| Iniciar la secuencia desde el principio | **Reproducir** |
| Detener/reanudar la secuencia | **Pausar / Continuar** |
| Dejar la demostración al inicio, detenida | **Reiniciar**; después pulsa **Reproducir** |
| Girar alrededor de la computadora | Arrastrar con botón izquierdo en la escena |
| Acercar/alejar | Rueda dentro de la escena |
| Separar las piezas | **Separar componentes** |
| Volver a sus posiciones | **Ensamblar** |
| Restablecer cámara | **Restablecer vista** o **R** |
| Desplazar una explicación larga | Rueda sobre el recuadro de explicación |
| Salir | **Esc** o cerrar la ventana |

El componente activo se ilumina y su marcador muestra el nombre. En exploración, su botón cambia de color. Mover el cursor más de cinco puntos durante una pulsación se considera un arrastre: gira la cámara sin seleccionar. Un clic selecciona al soltar. La distancia de cámara está entre 6,5 y 20 unidades; la elevación evita los polos. El panel se comprobó a 960 × 720 y muestra una indicación de lectura y una barra lateral; sólo el texto se desplaza.

En el recorrido, las teclas 1–5 y los marcadores no alteran el paso: se usan Anterior/Siguiente. En la demostración, un clic en una pieza o una tecla 1–5 vuelve a exploración libre para mostrar su explicación. Cambiar de modo detiene y reinicia el tiempo de la demostración. La posición separada/ensamblada y la cámara se conservan.

## Aprender cómo trabajan juntas las piezas

El recorrido visita **fuente → placa madre → SSD → RAM → procesador**. Cada paso usa los textos del componente y la misma selección que gobierna el resaltado y el nombre junto al marcador.

La demostración dura 12 segundos (tres fases de cuatro segundos):

1. El SSD conserva el programa y sus archivos.
2. Se cargan en la RAM las instrucciones y los datos necesarios.
3. El procesador utiliza esa información para ejecutar tareas y puede leer/escribir en la RAM.

Los **cuadrados y flechas turquesa** representan datos. La **línea dorada discontinua** representa el suministro de energía desde la fuente al sistema; la placa madre permite la comunicación. Son superposiciones educativas calculadas desde las posiciones actuales de las piezas, no dibujos de cables físicos. Funcionan también al separar, ensamblar o girar.

La RAM pierde su contenido al apagar; el SSD conserva sus archivos. La demostración lo recuerda de forma permanente y se identifica como una simplificación didáctica, sin simular tiempos reales de hardware, buses o cachés. Al finalizar se detiene. Reproducir vuelve a iniciar; Reiniciar vuelve al inicio sin reproducción automática. Pausar congela el tiempo y los símbolos; si se mueven las piezas o la cámara, sus rutas siguen ancladas a ellas.

Separar y ensamblar duran aproximadamente **1,25 segundos** desde un extremo al otro. Invertir la dirección conserva la posición actual. Restablecer vista sólo cambia la cámara.

## Comprobar el programa

En una sesión de escritorio con GPU:

```powershell
.\build\vs2026\Release\ExploraPC.exe --smoke
.\build\vs2026\Release\ExploraPC.exe --self-test
ctest --test-dir build/vs2026 -C Release --output-on-failure
```

`--smoke` comprueba dibujo, teclas y lógica educativa/cámara/animación. `--self-test` conserva las pruebas de v1 y añade selección por marcador, clic frente a arrastre, límites del recorrido, coincidencia de selección, pausa, continuación, reinicio, cambio de modo durante reproducción y rutas con piezas separadas. Ejecuta esta prueba sin intervenir con el ratón o teclado; dura unos 16 segundos si la pantalla refresca a 60 Hz.

La prueba completa usa un paso simulado de 1/30 s para ser reproducible; la aplicación normal utiliza tiempo real. Las capturas PPM se guardan en `evidencias` dentro de la carpeta desde donde se ejecuta la prueba y llevan prefijo `v2_`. Los PNG entregados son conversiones de esas capturas del framebuffer. En una ventana minimizada se pausa la actualización hasta que se pueda volver a dibujar.

Lee `COMPROBACIONES.md` para conocer el alcance de la verificación y las pruebas manuales pendientes.

## Archivos para el grupo

| Archivo | Contenido |
|---|---|
| `src/main.cpp` | Inicio, ventana, entradas y ciclo de dibujo |
| `src/Camara.h/.cpp` | Cámara orbital y límites |
| `src/Componentes.h/.cpp` | Textos, posiciones, geometrías y animación |
| `src/Dibujo.h/.cpp` | Cubo, buffers, shaders y gabinete |
| `src/InterfazEducativa.h/.cpp` | Panel, botones y marcadores |
| `src/Aprendizaje.h/.cpp` | Modos, pasos, fases, reloj y rutas de datos |
| `shaders/pieza.vert` y `pieza.frag` | Transformaciones, iluminación y patrones |
| `tests/PruebaGrafica.h` | Comprobación de interacción |
| `GUIA_APRENDIZAJE.md` | Conceptos gráficos explicados paso a paso |
| `DEPENDENCIAS.md` | Versiones, procedencia y licencias |

## Problemas habituales

- **No se encuentra un compilador**: instala la carga de trabajo C++ y ejecuta desde Developer PowerShell.
- **No se crea la ventana OpenGL 3.3**: instala el controlador gráfico del fabricante. Escritorio remoto y máquinas virtuales pueden ofrecer versiones inferiores.
- **No se pudo abrir el shader**: conserva `shaders/pieza.vert` y `shaders/pieza.frag` junto al ejecutable. CMake los copia en cada compilación.
- **Falta VCRUNTIME/MSVCP en otro equipo**: instala el [Microsoft Visual C++ Redistributable x64](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist). El ejecutable usa el runtime de MSVC, ya instalado en el equipo de desarrollo.
- **Una explicación no cabe**: desplaza ese recuadro con la rueda; los botones principales permanecen fuera del recuadro.

Referencias: [contextos GLFW](https://www.glfw.org/docs/3.3/context_guide.html), [integración de Dear ImGui y captura de entradas](https://github.com/ocornut/imgui/wiki/Getting-Started), [GLAD](https://github.com/Dav1dde/glad/tree/v2.0.8).

## Demostración en clase (2–3 minutos)

1. Haz clic en un marcador y muestra su nombre y función. Arrastra sobre él para mostrar que girar no selecciona.
2. Elige **Aprender paso a paso**, recorre las cinco piezas y explica la relación entre ellas.
3. Abre **¿Qué pasa cuando abro un programa?** y pulsa **Reproducir**.
4. Pausa al cargar la RAM. Diferencia cuadrados de datos y línea de energía; explica RAM temporal frente a SSD persistente.
5. Separa las piezas mientras está pausada, continúa hasta el procesador y muestra el intercambio CPU–RAM.
6. Reinicia y vuelve a explorar. Enseña `Aprendizaje.cpp` y la interpolación de las rutas para relacionarlo con el curso.
# Proyecto-ExploraPC
